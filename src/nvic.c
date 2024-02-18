#include "nvic.h"
#include "stm32f2xx_esl_nvic.h"
#include "stm32f2xx_esl_systick.h"

void NVIC_Init(void)
{
    // Enable global interrupt on timer 10
    ESL_NVIC_Enable_IRQ(TIM10_IRQn);

    // Enable IRQ on uart2
    ESL_NVIC_Enable_IRQ(UART2_IRQn);

    // Enable SysTick
    ESL_SysTick_Init(120000); // 120mhz clock / 120000 to get tick every 1ms
}