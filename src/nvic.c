#include "nvic.h"
#include "stm32f2xx_esl_nvic.h"
#include "stm32f2xx_esl_systick.h"

void NVIC_Init(void)
{
    // Enable global interrupt on timer 10
    ESL_NVIC_Enable(NVIC_TIM10_IRQ_POS);

    // Enable SysTick
    ESL_SysTick_Init(120000); // 120mhz clock / 120000 to get tick every 1ms
}