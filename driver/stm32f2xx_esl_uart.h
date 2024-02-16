/********************************************************************************************
 *  Filename: stm32f2xx_esl_uart.h
 *  Author: Erik Fagerland
 *  Created On: 15/02/2024
 * 
 *  Brief:
 *  Header file for uart.
 * 
 *******************************************************************************************/
#ifndef __STM32F2xx_ESL_USART_H
#define __STM32F2xx_ESL_USART_H

#include "stm32f207xx.h"
#include "stm32f2xx_esl_gpio.h"

// UARTx Base Adresses
#define UART1_BASE 0x40011000U
#define UART2_BASE 0x40004400U
#define UART3_BASE 0x40004800U
#define UART4_BASE 0x40004C00U
#define UART5_BASE 0x40005000U
#define UART6_BASE 0x40011400U

// UART Control Register 1 Defs
#define UART_CR1_SBK        (1 << 0)    // Send break characters
#define UART_CR1_RWU        (1 << 1)    // Receiver wakeup
#define UART_CR1_RE         (1 << 2)    // Receive Enable
#define UART_CR1_TE         (1 << 3)    // Transmit Enable
#define UART_CR1_IDLEIE     (1 << 4)    // IDLE Interrupt Enable
#define UART_CR1_RXNEIE     (1 << 5)    // RXNE Interrupt Enable
#define UART_CR1_TCIE       (1 << 6)    // TX Complete Interrupt Enable
#define UART_CR1_EXEIE      (1 << 7)    // TXE Interrupt Enable
#define UART_CR1_PEIE       (1 << 8)    // PE Interrupt Enable
#define UART_CR1_PS         (1 << 9)    // Parity Selection
#define UART_CR1_PCE        (1 << 10)   // Parity Control Enable
#define UART_CR1_WAKE       (1 << 11)   // Wakeup Method
#define UART_CR1_M          (1 << 12)   // Word length
#define UART_CR1_UE         (1 << 13)   // UART Enable
#define UART_CR1_OVER8      (1 << 15)   //Oversampling mode

// UART Control Register 2 Defs
#define UART_CR2_STOP_BIT_POS 12

// UART Status Register Defs
#define UART_SR_PE          (1 << 0)    // Parity Error
#define UART_SR_FE          (1 << 1)    // Framing Error
#define UART_SR_NF          (1 << 2)    // Noise Detected Flag
#define UART_SR_ORE         (1 << 3)    // Overrun Error
#define UART_SR_IDLE        (1 << 4)    // IDLE line detected
#define UART_SR_RXNE        (1 << 5)    // Read data register not empty
#define UART_SR_TC          (1 << 6)    // Transmission Complete
#define UART_SR_TXE         (1 << 7)    // Transmit data register empty
#define UART_SR_LBD         (1 << 8)    // LIN break detection flag
#define UART_SR_CTS         (1 << 9)    // CTS Flag

typedef struct
{
    UARTx_Typedef* instance;
    GPIO_Port_Typedef port;
} UARTx_Handle_Typedef;

// Enum for UART Word Length
typedef enum
{
    UART_WORD_LEN_8 = 0U,
    UART_WORD_LEN_9
} UART_WORD_LEN;

// Enum for UART Stop Bits
typedef enum
{
    UART_ONE_STOPBIT = 0U,
    UART_HALF_STOPBIT,
    UART_TWO_STOPBIT,
    UART_ONE_HALF_STOPBIT
} UART_STOPBITS;

// Enum for UART Baudrates
typedef enum
{
    BAUD_9600 = 9600U,
    BAUD_115200 = 115200U
} UART_BAUDRATE;

void ESL_UARTx_Init(UARTx_Handle_Typedef* UARTx, UART_BAUDRATE baud, UART_WORD_LEN wordlen, UART_STOPBITS stopbits);
ESL_StatusTypeDef ESL_UARTx_Write(UARTx_Handle_Typedef* UARTx, UInt8* buf, UInt32 length, UInt32 timeout);
ESL_StatusTypeDef ESL_UARTx_Read(UARTx_Handle_Typedef* UARTx, UInt8* buf, UInt32 length, UInt32 timeout);
ESL_StatusTypeDef ESL_UARTx_Flush(UARTx_Handle_Typedef* UARTx);

#endif // __STM32F2xx_ESL_USART_H