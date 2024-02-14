#include "nvic.h"
#include "stm32f2xx_esl_nvic.h"

void NVIC_Init(void)
{
    // Enable global interrupt on timer 10
    ESL_NVIC_Enable(NVIC_TIM10_IRQ_POS);
}