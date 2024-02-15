#include "stm32f2xx_esl_usart.h"

void ESL_UARTx_Init(UARTx_Typedef* UARTx, UART_BAUDRATE baud, UART_WORD_LEN len, UART_STOPBITS stopbits)
{
    // Enable USART
    UARTx->CR1 &= ~(1 << 13);
    UARTx->CR1 |= (1 << 13);

    // Set word length
    UARTx->CR1 &= ~(1 << 12);
    if (len == UART_WORD_LEN_9)
        UARTx->CR1 |= (1 << 12);
    
    // Set stop bits
    UARTx->CR2 &= ~(0b11UL << 12);
    UARTx->CR2 |= (stopbits << 12);

    // Set the baudrate
    UARTx->BRR = 30000000 / baud;

    UInt32 baudset = UARTx->BRR;

    // Enable transmitter
    UARTx->CR1 &= ~(1 << 3);
    UARTx->CR1 |= (1 << 3);
}

void ESL_UARTx_Write(UARTx_Typedef* UARTx, UInt8* buf, UInt32 length)
{
    UInt32 bytes_sent = 0;

    while (bytes_sent < length)
    {
        // Wait for TXE to set, indicating tx buffer is empty
        while (!(UARTx->SR & (1 << 7))){}

        // Set data in register
        UInt8 data = buf[bytes_sent];
        UARTx->DR = data;

        bytes_sent++;
    }

    // Wait for TC to set
    while (!(UARTx->SR & (1 << 6))){}
}