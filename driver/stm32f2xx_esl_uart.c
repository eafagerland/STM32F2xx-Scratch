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
void ESL_UARTx_Init(UARTx_Handle_Typedef* uart, UART_BAUDRATE baud, UART_WORD_LEN len, UART_STOPBITS stopbits)
{
    // Enable USART
    uart->instance->CR1 &= ~UART_CR1_UE;
    uart->instance->CR1 |= UART_CR1_UE;

    // Set word length
    uart->instance->CR1 &= ~UART_CR1_M;
    if (len == UART_WORD_LEN_9)
        uart->instance->CR1 |= UART_CR1_M;
    
    // Set stop bits
    uart->instance->CR2 &= ~(0b11UL << UART_CR2_STOP_BIT_POS);
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
}

/********************************************************************************************
 *  Writes data on given uart handle. A data pointer is provided with data to send and
 *  the length of the array. A timeout is also set and if elapsed it will return a timeout
 *  error.
 *******************************************************************************************/
ESL_StatusTypeDef ESL_UARTx_Write(UARTx_Handle_Typedef* uart, UInt8* buf, UInt32 length, UInt32 timeout)
{
    UInt32 bytes_sent = 0;
    UInt32 millis_started = ESL_Millis();

    while (bytes_sent < length)
    {
        // Calculate time spent waiting for TXE to be set
        UInt32 time_waiting = ESL_Millis() - millis_started;

        // Check for timeout
        if (time_waiting >= timeout)
            return ESL_TIMEOUT;


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
            return ESL_TIMEOUT;
    }

    return ESL_OK;
}

/********************************************************************************************
 *  Reads data on given uart handle. A data pointer is provided to store the data in.
 *  A timeout is also set and if elapsed it will return a timeout
 *  error. Returns when full length has been read.
 *******************************************************************************************/
ESL_StatusTypeDef ESL_UARTx_Read(UARTx_Handle_Typedef* uart, UInt8* buf, UInt32 length, UInt32 timeout)
{
    UInt32 bytes_read = 0;
    UInt32 millis_started = ESL_Millis();

    while (bytes_read < length)
    {
        // Calculate time spent waiting for TXE to be set
        UInt32 time_waiting = ESL_Millis() - millis_started;

        // Check for timeout
        if (time_waiting >= timeout)
            return ESL_TIMEOUT;

        // Wait for RXNE to set, indicating data in buffer     
        if (IS_BIT_SET(uart->instance->SR, UART_SR_RXNE))
        {
            // Read data register
            buf[bytes_read] = uart->instance->DR;
            bytes_read++;
        }
    }

    return ESL_OK;
}

/********************************************************************************************
 *  Flushes the uarts data register.
 *******************************************************************************************/
ESL_StatusTypeDef ESL_UARTx_Flush(UARTx_Handle_Typedef* uart)
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