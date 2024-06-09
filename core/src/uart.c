#include "uart.h"
#include "stm32f2xx_esl_uart.h"
#include "stm32f2xx_esl_nvic.h"

UARTx_Handle_TypeDef uart2 =
{
    .rx_state = UART_STATE_RESET,
    .tx_state = UART_STATE_RESET
};

void UART2_Init()
{
    uart2.instance = UART2;
    uart2.port = PORT_GPIOD;

    ESL_UARTx_Init(&uart2, BAUD_230400, UART_WORD_LEN_8, UART_ONE_STOPBIT);

    // Enable IRQ on uart2
    //ESL_NVIC_Enable_IRQ(UART2_IRQn);
}