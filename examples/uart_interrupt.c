#include "stm32f2xx_esl.h"
#include "gpio.h"
#include "nvic.h"
#include "uart.h"
#include "eslstring.h"

#define UART_RX_BUFF_SIZE 10
char rx_buf[UART_RX_BUFF_SIZE] = {'\0'};

#define UART_TX_BUFF_SIZE 100
char tx_buf[UART_TX_BUFF_SIZE] = {'\0'};

/***********************************************
 *	Function Prototypes
 ************************************************/
void HardFault_Handler(void);
static void init_system_clocks();

int main(void)
{
    init_system_clocks();
    ESL_Init();
    GPIO_Init();
    NVIC_Init();
    UART2_Init();

    // UART IRQ Read test
    ESL_UARTx_Receive_IT(&uart2, (UInt8 *)rx_buf, UART_RX_BUFF_SIZE);

    // Test UART IT TX
    stringcopy(tx_buf, "Hello World! Sent on TX IRQ\n\r");
    ESL_UARTx_Transmit_IT(&uart2, (UInt8 *)tx_buf, stringlen(tx_buf));

    while (1)
    {
        __wfi();
    }
}

/********************************************************************************************
 *  Receive Callback for Uarts
 *******************************************************************************************/
void ESL_UARTx_Receive_Callback(UARTx_Handle_TypeDef *uart)
{
    if (uart->instance == UART2)
    {
        print("UART CB! Data: ");
        char buf[100] = {'\0'};
        stringcopy(buf, (const char *)uart->rx_buf);
        println(buf);
        memset(rx_buf, '\0', UART_RX_BUFF_SIZE);
        ESL_UARTx_Receive_IT(&uart2, (UInt8 *)rx_buf, UART_RX_BUFF_SIZE);
    }
}

/********************************************************************************************
 *  Transmit Callback for Uarts
 *******************************************************************************************/
void ESL_UARTx_Transmit_Callback(UARTx_Handle_TypeDef *uart)
{
    if (uart->instance == UART2)
    {
        print("TX DONE!\r\n");
        ESL_GPIO_WritePin(LED_PORT, RED_LED, GPIO_PIN_RESET);
    }
}