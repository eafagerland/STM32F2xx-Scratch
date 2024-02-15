#include "stm32f2xx_esl.h"
#include "gpio.h"
#include "nvic.h"
#include "tim.h"
#include "usart.h"

#define BLUE_LED GPIO_PIN_7
#define RED_LED GPIO_PIN_14
#define USER_BUTTON GPIO_PIN_13

#define FLASH_BASE 0x40023C00U

#define REG(x) (*(UInt32 *)x)

int main(void)
{
	REG(FLASH_BASE) |= (90 << 0); // Set flash latency 3WS

	ESL_RCC_Init(0, 240, 8, 4, RCC_APBx_CLOCK_DIV4, RCC_APBx_CLOCK_DIV2, RCC_AHB_CLOCK_DIV0);
	GPIO_Init();
	TIM_Init();
	NVIC_Init();
	UART2_Init();

	UInt16 timer_val_tim11 = TIM11->CNT;

	// Delay test
	for (int i = 0; i < 10; i++)
	{
		ESL_GPIO_WritePin(GPIOB, RED_LED, GPIO_PIN_SET);
		ESL_Delay(100);
		ESL_GPIO_WritePin(GPIOB, RED_LED, GPIO_PIN_RESET);
		ESL_Delay(100);
	}

	// UART Test
	for (int i = 0; i < 5; i++)
	{
		const char* buf = "Hello world!\n\r";
		ESL_UARTx_Write(UART2, (UInt8*)buf, 15);
		ESL_Delay(1000);
	}

	ESL_GPIO_WritePin(GPIOB, RED_LED, GPIO_PIN_SET);

	while (1)
	{
		// Button test
		/*if (ESL_GPIO_Read_Pinstate(GPIOC, USER_BUTTON) == GPIO_PIN_SET)
		{
			ESL_GPIO_TogglePin(GPIOB, RED_LED);
			ESL_Delay(500);
		}*/
		/*if (TIM11->CNT - timer_val_tim11 >= 10000) // Toggle led every 1sec
		{
			ESL_GPIO_TogglePin(GPIOB, RED_LED);
			timer_val_tim11 = TIM11->CNT;
		}*/
	}
}

void EXTI15_10_Handler(void)
{
	ESL_GPIO_TogglePin(GPIOB, RED_LED);
	EXTI->PR |= (1 << 13);  // Reset interrupt
}

void ESL_TIM_IRQ_Handler(TIMx_Typedef* TIMx)
{
	if (TIMx == TIM10)
	{
		ESL_GPIO_TogglePin(GPIOB, BLUE_LED);

		// Reset interrupt
		ESL_TIM_Reset_IRQ(TIM10);
	}
}

void HardFault_Handler(void)
{
	ESL_GPIO_WritePin(GPIOB, RED_LED, GPIO_PIN_SET);
	ESL_GPIO_WritePin(GPIOB, BLUE_LED, GPIO_PIN_SET);
}
