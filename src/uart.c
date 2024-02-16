#include "uart.h"
#include "stm32f2xx_esl_uart.h"

UARTx_Handle_Typedef uart2;

void UART2_Init()
{
    uart2.instance = UART2;
    uart2.port = PORT_GPIOD;

    ESL_UARTx_Init(&uart2, BAUD_9600, UART_WORD_LEN_8, UART_ONE_STOPBIT);
}