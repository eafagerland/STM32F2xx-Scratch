#include "interrupts.h"

#include "stm32f2xx_esl_uart.h"

extern UARTx_Handle_TypeDef uart2;

/********************************************************************************************
 *  Handles the Global Interrupt for UART2
 *******************************************************************************************/
void UART2_IRQ_Handler(void)
{
    ESL_UARTx_IRQ_Handler(&uart2);
}