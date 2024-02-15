#include "stm32f2xx_esl_uart.h"
#include "stm32f2xx_esl_rcc.h"
#include "stm32f2xx_esl_systick.h"

void ESL_UARTx_Init(UARTx_Typedef* UARTx, UART_BAUDRATE baud, UART_WORD_LEN len, UART_STOPBITS stopbits)
{
    // Enable USART
    UARTx->CR1 &= ~UART_CR1_UE;
    UARTx->CR1 |= UART_CR1_UE;

    // Set word length
    UARTx->CR1 &= ~UART_CR1_M;
    if (len == UART_WORD_LEN_9)
        UARTx->CR1 |= UART_CR1_M;
    
    // Set stop bits
    UARTx->CR2 &= ~(0b11UL << UART_CR2_STOP_BIT_POS);
    UARTx->CR2 |= (stopbits << UART_CR2_STOP_BIT_POS);

    // Set the baudrate
    UARTx->BRR = RCC_Clocks.APB1_CLOCK / baud;

    // Enable transmitter
    UARTx->CR1 &= ~UART_CR1_TE;
    UARTx->CR1 |= UART_CR1_TE;

    // Enable receiver
    UARTx->CR1 &= ~UART_CR1_RE;
    UARTx->CR1 |= UART_CR1_RE;
}

ESL_StatusTypeDef ESL_UARTx_Write(UARTx_Typedef* UARTx, UInt8* buf, UInt32 length, UInt32 timeout)
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
        if (UARTx->SR & UART_SR_TXE)
        {
            // Set data in register
            UInt8 data = buf[bytes_sent];
            UARTx->DR = data;
            bytes_sent++;
        }
    }

    // Wait for transmission complete
    while (!(UARTx->SR & UART_SR_TC))
    {
        // Calculate time spent waiting for TC to be set
        UInt32 time_waiting = ESL_Millis() - millis_started;

        // Check for timeout
        if (time_waiting >= timeout)
            return ESL_TIMEOUT;
    }

    return ESL_OK;
}

ESL_StatusTypeDef ESL_UARTx_Read(UARTx_Typedef* UARTx, UInt8* buf, UInt32 length, UInt32 timeout)
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
        if (UARTx->SR & UART_SR_RXNE)
        {
            // Read data register
            buf[bytes_read] = UARTx->DR;
            bytes_read++;
        }
    }

    return ESL_OK;
}

ESL_StatusTypeDef ESL_UARTx_Flush(UARTx_Typedef* UARTx)
{
    volatile UInt8 dummy_byte = 0;
    UInt32 millis_started = ESL_Millis();

    // Read from register until empty
    while (UARTx->SR & UART_SR_RXNE)
    {
        // Calculate time spent waiting for TXE to be set
        UInt32 time_waiting = ESL_Millis() - millis_started;

        // Check for timeout
        if (time_waiting >= 5000)
            return ESL_TIMEOUT;

        dummy_byte = UARTx->DR;
        (void)dummy_byte; // Supress compiler warning
    }

    return ESL_OK;
}