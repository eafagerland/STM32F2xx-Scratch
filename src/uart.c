#include "uart.h"
#include "stm32f2xx_esl_uart.h"

void UART2_Init()
{
    ESL_UARTx_Init(UART2, BAUD_9600, UART_WORD_LEN_8, UART_ONE_STOPBIT);
}