/********************************************************************************************
 *  Filename: stm32f2xx_esl_uart.h
 *  Author: Erik Fagerland
 *  Created On: 15/02/2024
 *
 *  Brief:
 *  Header file for uart.
 *
 *******************************************************************************************/
#ifndef __STM32F2xx_ESL_UART_H
#define __STM32F2xx_ESL_UART_H

#include "stm32f207xx.h"
#include "stm32f2xx_esl_gpio.h"

// UARTx Base Adresses
#define UART1_BASE          (0x40011000UL)
#define UART2_BASE          (0x40004400UL)
#define UART3_BASE          (0x40004800UL)
#define UART4_BASE          (0x40004C00UL)
#define UART5_BASE          (0x40005000UL)
#define UART6_BASE          (0x40011400UL)

// UART Control Register 1 Defs
#define UART_CR1_SBK        (1U << 0U)  // Send break characters
#define UART_CR1_RWU        (1U << 1U)  // Receiver wakeup
#define UART_CR1_RE         (1U << 2U)  // Receive Enable
#define UART_CR1_TE         (1U << 3U)  // Transmit Enable
#define UART_CR1_IDLEIE     (1U << 4U)  // IDLE Interrupt Enable
#define UART_CR1_RXNEIE     (1U << 5U)  // RXNE Interrupt Enable
#define UART_CR1_TCIE       (1U << 6U)  // TX Complete Interrupt Enable
#define UART_CR1_TXEIE      (1U << 7U)  // TXE Interrupt Enable
#define UART_CR1_PEIE       (1U << 8U)  // PE Interrupt Enable
#define UART_CR1_PS         (1U << 9U)  // Parity Selection
#define UART_CR1_PCE        (1U << 10U) // Parity Control Enable
#define UART_CR1_WAKE       (1U << 11U) // Wakeup Method
#define UART_CR1_M          (1U << 12U) // Word length
#define UART_CR1_UE         (1U << 13U) // UART Enable
#define UART_CR1_OVER8      (1U << 15U) // Oversampling mode

// UART Control Register 2 Defs
#define UART_CR2_STOP_BIT_POS (12U)

// UART Status Register Defs
#define UART_SR_PE          (1U << 0U)  // Parity Error
#define UART_SR_FE          (1U << 1U)  // Framing Error
#define UART_SR_NF          (1U << 2U)  // Noise Detected Flag
#define UART_SR_ORE         (1U << 3U)  // Overrun Error
#define UART_SR_IDLE        (1U << 4U)  // IDLE line detected
#define UART_SR_RXNE        (1U << 5U)  // Read data register not empty
#define UART_SR_TC          (1U << 6U)  // Transmission Complete
#define UART_SR_TXE         (1U << 7U)  // Transmit data register empty
#define UART_SR_LBD         (1U << 8U)  // LIN break detection flag
#define UART_SR_CTS         (1U << 9U)  // CTS Flag

typedef enum
{
    UART_STATE_RESET = 0U,
    UART_STATE_READY,
    UART_STATE_BUSY
} UART_State_TypeDef;

typedef struct
{
    UARTx_TypeDef *instance;
    GPIO_Port_TypeDef port;
    UInt8 *rx_buf;
    UInt32 rx_buf_pos;
    UInt32 rx_buf_len;
    UInt8 *tx_buf;
    UInt32 tx_buf_pos;
    UInt32 tx_buf_len;
    UART_State_TypeDef rx_state;
    UART_State_TypeDef tx_state;
} UARTx_Handle_TypeDef;

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

void ESL_UARTx_Init(UARTx_Handle_TypeDef *UARTx, UART_BAUDRATE baud, UART_WORD_LEN wordlen, UART_STOPBITS stopbits);
ESL_StatusTypeDef ESL_UARTx_Transmit(UARTx_Handle_TypeDef *uart, UInt8 *buf, UInt32 length, UInt32 timeout);
ESL_StatusTypeDef ESL_UARTx_Receive(UARTx_Handle_TypeDef *uart, UInt8 *buf, UInt32 length, UInt32 timeout);
ESL_StatusTypeDef ESL_UARTx_Flush(UARTx_Handle_TypeDef *uart);

ESL_StatusTypeDef ESL_UARTx_Receive_To_Idle(UARTx_Handle_TypeDef *uart, UInt8 *buf, UInt32 length, UInt32 timeout);
ESL_StatusTypeDef ESL_UARTx_Receive_To_Idle_IT(UARTx_Handle_TypeDef *uart, UInt8 *buf, UInt32 length);
ESL_StatusTypeDef ESL_UARTx_Receive_IT(UARTx_Handle_TypeDef *uart, UInt8 *buf, UInt32 length);
ESL_StatusTypeDef ESL_UARTx_Transmit_IT(UARTx_Handle_TypeDef *uart, UInt8 *buf, UInt32 length);
void ESL_UARTx_Receive_Callback(UARTx_Handle_TypeDef *uart);
void ESL_UARTx_Transmit_Callback(UARTx_Handle_TypeDef *uart);
void ESL_UARTx_IRQ_Handler(UARTx_Handle_TypeDef *uart);

#endif // __STM32F2xx_ESL_UART_H