/********************************************************************************************
 *  Filename: stm32f2xx_esl_uart.h
 *  Author: Erik Fagerland
 *  Created On: 15/02/2024
 * 
 *  Brief:
 *  Implementation of UARTs on the MCU
 * 
 *******************************************************************************************/
#include "stm32f2xx_esl_uart.h"
#include "stm32f2xx_esl_rcc.h"
#include "stm32f2xx_esl_systick.h"

/********************************************************************************************
 *  Initializes given uart handle with given baud, length and stopbits. Enables both
 *  receiver and transmitter.
 *******************************************************************************************/
void ESL_UARTx_Init(UARTx_Handle_TypeDef* uart, UART_BAUDRATE baud, UART_WORD_LEN len, UART_STOPBITS stopbits)
{
    // Enable USART
    uart->instance->CR1 &= ~UART_CR1_UE;
    uart->instance->CR1 |= UART_CR1_UE;

    // Set word length
    uart->instance->CR1 &= ~UART_CR1_M;
    if (len == UART_WORD_LEN_9)
        uart->instance->CR1 |= UART_CR1_M;
    
    // Set stop bits
    uart->instance->CR2 &= ~(0x3UL << UART_CR2_STOP_BIT_POS);
    uart->instance->CR2 |= (stopbits << UART_CR2_STOP_BIT_POS);

    // Set the baudrate
    uart->instance->BRR = RCC_Clocks.APB1_CLOCK / baud;

    // Enable transmitter
    uart->instance->CR1 &= ~UART_CR1_TE;
    uart->instance->CR1 |= UART_CR1_TE;

    // Enable receiver
    uart->instance->CR1 &= ~UART_CR1_RE;
    uart->instance->CR1 |= UART_CR1_RE;

    // Flush buffers
    ESL_UARTx_Flush(uart);

    uart->rx_state = UART_STATE_READY;
    uart->tx_state = UART_STATE_READY;
}

/********************************************************************************************
 *  Writes data on given uart handle. A data pointer is provided with data to send and
 *  the length of the array. A timeout is also set and if elapsed it will return a timeout
 *  error.
 *******************************************************************************************/
ESL_StatusTypeDef ESL_UARTx_Transmit(UARTx_Handle_TypeDef* uart, UInt8* buf, UInt32 length, UInt32 timeout)
{
    // Return error if not ready
    if (uart->tx_state != UART_STATE_READY)
        return ESL_BUSY;

    UInt32 bytes_sent = 0;
    UInt32 millis_started = ESL_Millis();
    uart->tx_state = UART_STATE_BUSY;

    while (bytes_sent < length)
    {
        // Calculate time spent waiting for TXE to be set
        UInt32 time_waiting = ESL_Millis() - millis_started;

        // Check for timeout
        if (time_waiting >= timeout)
        {
            uart->tx_state = UART_STATE_READY;
            return ESL_TIMEOUT;
        }


        // Wait for TXE to set, indicating TX buffer is empty       
        if (IS_BIT_SET(uart->instance->SR, UART_SR_TXE))
        {
            // Set data in register
            UInt8 data = buf[bytes_sent];
            uart->instance->DR = data;
            bytes_sent++;
        }
    }

    // Wait for transmission complete
    while (!(IS_BIT_SET(uart->instance->SR, UART_SR_TC)))
    {
        // Calculate time spent waiting for TC to be set
        UInt32 time_waiting = ESL_Millis() - millis_started;

        // Check for timeout
        if (time_waiting >= timeout)
        {
            uart->tx_state = UART_STATE_READY;
            return ESL_TIMEOUT;
        }
    }

    uart->tx_state = UART_STATE_READY;
    return ESL_OK;
}

/********************************************************************************************
 *  Reads data on given uart handle. A data pointer is provided to store the data in.
 *  A timeout is also set and if elapsed it will return a timeout
 *  error. Returns when full length has been read.
 *******************************************************************************************/
ESL_StatusTypeDef ESL_UARTx_Receive(UARTx_Handle_TypeDef* uart, UInt8* buf, UInt32 length, UInt32 timeout)
{
    // Return error if not ready
    if (uart->rx_state != UART_STATE_READY)
        return ESL_BUSY;

    UInt32 bytes_read = 0;
    UInt32 millis_started = ESL_Millis();
    uart->rx_state = UART_STATE_BUSY;

    while (bytes_read < length)
    {
        // Calculate time spent waiting for TXE to be set
        UInt32 time_waiting = ESL_Millis() - millis_started;

        // Check for timeout
        if (time_waiting >= timeout)
        {
            uart->rx_state = UART_STATE_READY;
            return ESL_TIMEOUT;
        }

        // Wait for RXNE to set, indicating data in buffer     
        if (IS_BIT_SET(uart->instance->SR, UART_SR_RXNE))
        {
            // Read data register
            buf[bytes_read] = uart->instance->DR;
            bytes_read++;
        }
    }

    uart->rx_state = UART_STATE_READY;
    return ESL_OK;
}

/********************************************************************************************
 *  Flushes the uarts data register.
 *******************************************************************************************/
ESL_StatusTypeDef ESL_UARTx_Flush(UARTx_Handle_TypeDef* uart)
{
    volatile UInt8 dummy_byte = 0;
    UInt32 millis_started = ESL_Millis();

    // Read from register until empty  
    while (IS_BIT_SET(uart->instance->SR, UART_SR_RXNE))
    {
        // Calculate time spent waiting for TXE to be set
        UInt32 time_waiting = ESL_Millis() - millis_started;

        // Check for timeout
        if (time_waiting >= 5000)
            return ESL_TIMEOUT;

        dummy_byte = uart->instance->DR;
        (void)dummy_byte; // Supress compiler warning
    }
    return ESL_OK;
}

/********************************************************************************************
 *  Starts to receive in blocking mode until given length has been read from buffer or
 *  an idle line was detected.
 *******************************************************************************************/
ESL_StatusTypeDef ESL_UARTx_Receive_To_Idle(UARTx_Handle_TypeDef* uart, UInt8* buf, UInt32 length, UInt32 timeout)
{
    // TODO: Implement
    return ESL_ERROR;
}

/********************************************************************************************
 *  Starts the IRQ receive to Idle on UART. Callback will be called when given length has
 *  been read or an idle line was detected.
 *******************************************************************************************/
ESL_StatusTypeDef ESL_UARTx_Receive_To_Idle_IT(UARTx_Handle_TypeDef* uart, UInt8* buf, UInt32 length)
{    
    // Return error if not ready
    if (uart->rx_state != UART_STATE_READY)
        return ESL_BUSY;

    // Set the pointers to RX buf so it can be stored during IRQ reads
    uart->rx_buf = buf;
    uart->rx_buf_pos = 0;
    uart->rx_buf_len = length;
    uart->rx_state = UART_STATE_BUSY;

    // Enable IDLE interrupt
    RESET_REG(uart->instance->CR1, UART_CR1_IDLEIE);
    SET_REG(uart->instance->CR1, UART_CR1_IDLEIE);

    return ESL_OK;
}

/********************************************************************************************
 *  Starts the IRQ receive on UART. Callback will be called when given length has been read.
 *******************************************************************************************/
ESL_StatusTypeDef ESL_UARTx_Receive_IT(UARTx_Handle_TypeDef* uart, UInt8* buf, UInt32 length)
{
    // Return error if not ready
    if (uart->rx_state != UART_STATE_READY)
        return ESL_BUSY;

    // Set the pointers to RX buf so it can be stored during IRQ reads
    uart->rx_buf = buf;
    uart->rx_buf_pos = 0;
    uart->rx_buf_len = length;
    uart->rx_state = UART_STATE_BUSY;

    // Enable RX interrupt
    RESET_REG(uart->instance->CR1, UART_CR1_RXNEIE);
    SET_REG(uart->instance->CR1, UART_CR1_RXNEIE);

    return ESL_OK;
}

/********************************************************************************************
 *  Starts IRQ transmission on UART. Callback is called when given length of bytes have
 *  been sent.
 *******************************************************************************************/
ESL_StatusTypeDef ESL_UARTx_Transmit_IT(UARTx_Handle_TypeDef* uart, UInt8* buf, UInt32 length)
{
    // Return error if not ready
    if (uart->tx_state != UART_STATE_READY)
        return ESL_BUSY;

    // Set the pointers to RX buf so it can be stored during IRQ reads
    uart->tx_buf = buf;
    uart->tx_buf_pos = 0;
    uart->tx_buf_len = length;
    uart->tx_state = UART_STATE_BUSY;

    // Enable TX complete interrupt
    RESET_REG(uart->instance->CR1, UART_CR1_TCIE);
    SET_REG(uart->instance->CR1, UART_CR1_TCIE);

    // Enable TX empty interrupt
    RESET_REG(uart->instance->CR1, UART_CR1_TXEIE);
    SET_REG(uart->instance->CR1, UART_CR1_TXEIE);

    return ESL_OK;
}

/********************************************************************************************
 *  Weak declared UART receive callback. If the RX IRQ was enabled on the UART this callback
 *  will be called whenever the RX has completed or idle line detected if activated.
 *  To be implemented in user files.
 *******************************************************************************************/
__weak void ESL_UARTx_Receive_Callback(UARTx_Handle_TypeDef* uart)
{
    UNUSED(uart);
    /* NOTE: 
     * This function should not be modified, when the callback is needed it can be implemented in user file
    */
}

/********************************************************************************************
 *  Weak declared UART transmit callback. If TX IRQ was enabled on the UART this callback
 *  will be called whenever the TX has completed. 
 *  To be implemented in user files.
 *******************************************************************************************/
__weak void ESL_UARTx_Transmit_Callback(UARTx_Handle_TypeDef* uart)
{
    UNUSED(uart);
    /* NOTE: 
     * This function should not be modified, when the callback is needed it can be implemented in user file
    */
}

void handle_receive_irq()
{

}

/********************************************************************************************
 *  Global interrupt handler for UARTs.
 *  Handles all the IRQ received from UARTs.
 *******************************************************************************************/
void ESL_UARTx_IRQ_Handler(UARTx_Handle_TypeDef* uart)
{
    UInt32 sr_reg = uart->instance->SR;
    UInt32 cr1_reg = uart->instance->CR1;

    /***************************************************************************************
    *  Transmit Handling
    */
    Bool is_tx_interrupt_en     = IS_BIT_SET(cr1_reg, UART_CR1_TXEIE);
    Bool is_tx_complete_en      = IS_BIT_SET(cr1_reg, UART_CR1_TCIE);
    Bool is_tx_ready            = IS_BIT_SET(sr_reg, UART_SR_TXE);
    Bool is_tx_complete         = IS_BIT_SET(sr_reg, UART_SR_TC);

    // Test if at last byte to send
    if (uart->tx_buf_pos >= (uart->tx_buf_len - 1) && is_tx_interrupt_en)
    {
        uart->instance->DR = uart->tx_buf[uart->tx_buf_pos];
        RESET_REG(uart->instance->CR1, UART_CR1_TXEIE);
        return;
    }

    // Check if register empty and ready to send new byte
    if (is_tx_ready && is_tx_interrupt_en)
    {
        uart->instance->DR = uart->tx_buf[uart->tx_buf_pos];
        uart->tx_buf_pos++;
        return;
    }

    // Check if transmission complete
    if (is_tx_complete && is_tx_complete_en)
    {
        uart->tx_state = UART_STATE_READY;
        RESET_REG(uart->instance->CR1, UART_CR1_TCIE);
        ESL_UARTx_Transmit_Callback(uart);
        return;
    }

    /***************************************************************************************
    *  Receive Handling
    */
    Bool is_idle_detected       = IS_BIT_SET(sr_reg, UART_SR_IDLE) && IS_BIT_SET(cr1_reg, UART_CR1_IDLEIE);
    Bool is_data_available      = IS_BIT_SET(sr_reg, UART_SR_RXNE);
    Bool is_rx_interrupt_en     = IS_BIT_SET(cr1_reg, UART_CR1_RXNEIE);

    // Test if at max buffer size
    if (uart->rx_buf_pos >= (uart->rx_buf_len - 1) && is_rx_interrupt_en) // Minus one since we are reading the last byte now
    {
        uart->rx_buf[uart->rx_buf_pos] = uart->instance->DR;
        uart->rx_state = UART_STATE_READY;
        RESET_REG(uart->instance->CR1, UART_CR1_RXNEIE);
        ESL_UARTx_Receive_Callback(uart);
        return;
    }

    // Check if data available and cleared to read
    if (is_data_available && is_rx_interrupt_en)
    {
        uart->rx_buf[uart->rx_buf_pos] = uart->instance->DR;
        uart->rx_buf_pos++;
        return;
    }

    // Check for idle line
    if (is_idle_detected)
    {
        uart->rx_buf[uart->rx_buf_pos] = uart->instance->DR;
        uart->rx_state = UART_STATE_READY;
        RESET_REG(uart->instance->CR1, UART_CR1_IDLEIE);
        ESL_UARTx_Receive_Callback(uart);
        return;
    }
}