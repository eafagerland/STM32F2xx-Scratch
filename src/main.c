#include "stm32f2xx_esl.h"

/* Offset Addresses for Peripherals */
#define RCC_CR_OFFSET       0x00U   // Control register for clock and reset control
#define RCC_CFGR_OFFSET     0x08U   // Configuration register for clock and reset control
#define RCC_AHB1ENR_OFFSET  0x30U   // Offset address for advanced peripheral bus
#define RCC_APB1ENR_OFFSET  0x40U	// Offset address for advanced peripheral bus 1	
#define RCC_APB2ENR_OFFSET  0x44U   // Offset address for advanced peripheral bus 2

/* Adding the peripheral base address with offset of some
 * register will take you to the actual address */
#define RCC_CR          (RCC_BASE + RCC_CR_OFFSET)
#define RCC_CFGR        (RCC_BASE + RCC_CFGR_OFFSET)
#define RCC_AHB1ENR     (RCC_BASE + RCC_AHB1ENR_OFFSET)
#define RCC_APB1ENR     (RCC_BASE + RCC_APB1ENR_OFFSET)
#define RCC_APB2ENR     (RCC_BASE + RCC_APB2ENR_OFFSET)

#define FLASH_BASE 0x40023C00U

#define REG(x) (*(UInt32 *)x)

/* spin for some amount of ticks */
void delay(volatile Int32  t)
{
	for (volatile Int32 i = 0; i < t; i++) {};
}

int main(void)
{
	// Enable HSE
	//REG(RCC_CR) |= (1 << 16);

	// Check stable clock and set HSI as source (will set 16Mhz as default clock)
	//if (REG(RCC_CR) & (1 << 17))
	//	REG(RCC_CFGR) |= (1 << 0);

	REG(FLASH_BASE) |= (90 << 0); // Set flash latency 3WS
	ESL_RCC_Init(0, 240, 8, RCC_APBx_CLOCK_DIV4, RCC_APBx_CLOCK_DIV2, RCC_AHB_CLOCK_DIV0);
	GPIO_Init();
	TIM_Init();
	NVIC_Init();

	UInt16 timer_val_tim11 = TIM11->CNT;

	while (1)
	{
		if (TIM11->CNT - timer_val_tim11 >= 10000) // Toggle led every 1sec
		{
			ESL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);
			timer_val_tim11 = TIM11->CNT;
		}
	}
}

void TIM10_IRQHandler(void)
{
	ESL_GPIO_TogglePin(GPIOB, GPIO_PIN_14);

	// Reset interrupt
	TIM10->SR = 0U;
}

void HardFault_Handler(void)
{
	ESL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
	ESL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
}
