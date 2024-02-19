#include "stm32f2xx_esl.h"
#include "gpio.h"
#include "nvic.h"
#include "tim.h"
#include "uart.h"
#include "eslstring.h"
#include "rtc.h"

#define BLUE_LED GPIO_PIN_7
#define RED_LED GPIO_PIN_14
#define USER_BUTTON GPIO_PIN_13

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
	TIM_Init();
	NVIC_Init();
	UART2_Init();
	RTC_Init();

	// Init complete!
	print("Init Complete!\n\r");

	// Check if it started from a deepsleep standby
	if (ESL_PWR_Standby_Flagged())
	{
		print("Awww.. I was just sleeping!\n\r");
	}

	// Delay test
	for (int i = 0; i < 10; i++)
	{
		ESL_GPIO_TogglePin(GPIOB, RED_LED);
		ESL_Delay(100);
	}

	// Print something fancy in terminal
	print("Enter something!\n\r");

	//UART IRQ Read test
	ESL_UARTx_Receive_IT(&uart2, (UInt8*)rx_buf, UART_RX_BUFF_SIZE);

	// Test UART IT TX
	stringcopy(tx_buf, "Hello World! Sent on TX IRQ\n\r");
	ESL_UARTx_Transmit_IT(&uart2, (UInt8*)tx_buf, stringlen(tx_buf));

	// Enable wakeup pin
	//ESL_PWR_Enable_WKUP_Pin();
	// Enter Stop mode deepsleep (wakeup with user button)
	//ESL_PWR_Enter_Sleep(PWR_SLP_PPDS_STB, PWR_SLP_LPDS_OFF); 

	while (1)
	{
		__wfi();
	}
}

/********************************************************************************************
 *  Initializes the system clocks.
 *******************************************************************************************/
static void init_system_clocks()
{
	// Init system clocks, go to hardfault handler if there was errors in settings (use STM32MXCube to get these values)
	if (ESL_RCC_Init(RCC_PLLP_CLOCK_DIV2, 240, 8, 4, RCC_APBx_CLOCK_DIV4, RCC_APBx_CLOCK_DIV2, RCC_AHB_CLOCK_DIV1) != ESL_OK)
		HardFault_Handler();
}

/********************************************************************************************
 *  Callback for GPIO external interrupt line 15-10
 *******************************************************************************************/
void EXTI15_10_Handler(void)
{
	// Check if PIN13 was the source of the interrupt
	if (GPIO_EXTI_SOURCE(GPIO_PIN_13))
	{
		if (g_pwr_stop_mode_active)
		{
			g_pwr_stop_mode_active = FALSE;
			init_system_clocks(); // Need to re-init clocks since they were disabled at sleep
			ESL_SysTick_Resume(); // Resume the systick
			print("Woke up from sleep mode!\n\r");
		}
		else
		{
			if (ESL_UARTx_Transmit_IT(&uart2, (UInt8*)tx_buf, stringlen(tx_buf)) == ESL_OK)
				ESL_GPIO_WritePin(GPIOB, RED_LED, GPIO_PIN_SET);
		}
		EXTI->PR |= GPIO_PIN_13;  // Reset interrupt
	}	
}

/********************************************************************************************
 *  Callback for timers
 *******************************************************************************************/
void ESL_TIM_IRQ_Handler(TIMx_TypeDef* TIMx)
{
	if (TIMx == TIM10)
	{
		// Print RTC Seconds Test
		char buf[50] = {'\0'};
		// Extract the high and low nibbles
		UInt8 high_nibble = (UInt8)(RTC->TR >> 4) & 0x0F;
		UInt8 low_nibble = (UInt8)(RTC->TR & 0x0F);
		UInt8 seconds = (high_nibble * 10) + low_nibble;
		uint_to_string(seconds, buf);
		print("Seconds: ");
		println(buf);

		ESL_GPIO_TogglePin(GPIOB, BLUE_LED);

		// Reset interrupt
		ESL_TIM_Reset_IRQ(TIM10);
	}
}

/********************************************************************************************
 *  Hardfault handler
 *******************************************************************************************/
void HardFault_Handler(void)
{
	ESL_GPIO_WritePin(GPIOB, RED_LED, GPIO_PIN_SET);
	ESL_GPIO_WritePin(GPIOB, BLUE_LED, GPIO_PIN_SET);
	while (1){}
}

/********************************************************************************************
 *  Receive Callback for Uarts
 *******************************************************************************************/
void ESL_UARTx_Receive_Callback(UARTx_Handle_TypeDef* uart)
{
	if (uart->instance == UART2)
	{
		print("UART CB! Data: ");
		char buf[100] = {'\0'};
		stringcopy(buf, (const char*)uart->rx_buf);
		println(buf);
		memset(rx_buf, '\0', UART_RX_BUFF_SIZE);
		ESL_UARTx_Receive_IT(&uart2, (UInt8*)rx_buf, UART_RX_BUFF_SIZE);
	}
}

/********************************************************************************************
 *  Transmit Callback for Uarts
 *******************************************************************************************/
void ESL_UARTx_Transmit_Callback(UARTx_Handle_TypeDef* uart)
{
	if (uart->instance == UART2)
	{
		print("TX DONE!\r\n");
		ESL_GPIO_WritePin(GPIOB, RED_LED, GPIO_PIN_RESET);
	}
}