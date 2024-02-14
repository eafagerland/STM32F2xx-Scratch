#include "stm32f2xx_esl.h"

#define BLUE_LED GPIO_PIN_7
#define RED_LED GPIO_PIN_14

#define FLASH_BASE 0x40023C00U

#define REG(x) (*(UInt32 *)x)

int main(void)
{
	REG(FLASH_BASE) |= (90 << 0); // Set flash latency 3WS

	ESL_RCC_Init(0, 240, 8, 4, RCC_APBx_CLOCK_DIV4, RCC_APBx_CLOCK_DIV2, RCC_AHB_CLOCK_DIV0);
	GPIO_Init();
	TIM_Init();
	NVIC_Init();

	UInt16 timer_val_tim11 = TIM11->CNT;

	// Delay test
	for (int i = 0; i < 10; i++)
	{
		ESL_GPIO_WritePin(GPIOB, RED_LED, GPIO_PIN_SET);
		ESL_Delay(100);
		ESL_GPIO_WritePin(GPIOB, RED_LED, GPIO_PIN_RESET);
		ESL_Delay(100);
	}

	while (1)
	{
		if (ESL_Millis() >= 10000)
		{
			ESL_GPIO_WritePin(GPIOB, RED_LED, GPIO_PIN_SET);
		}
		/*if (TIM11->CNT - timer_val_tim11 >= 10000) // Toggle led every 1sec
		{
			ESL_GPIO_TogglePin(GPIOB, RED_LED);
			timer_val_tim11 = TIM11->CNT;
		}*/
	}
}

void TIM10_IRQHandler(void)
{
	ESL_GPIO_TogglePin(GPIOB, BLUE_LED);

	// Reset interrupt
	TIM10->SR = 0U;
}

void HardFault_Handler(void)
{
	ESL_GPIO_WritePin(GPIOB, RED_LED, GPIO_PIN_SET);
	ESL_GPIO_WritePin(GPIOB, BLUE_LED, GPIO_PIN_SET);
}
