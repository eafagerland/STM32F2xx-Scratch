#include "tim.h"
#include "stm32f2xx_esl_timer.h"
#include "stm32f2xx_esl_nvic.h"

void TIM_Init(void)
{
    ESL_TIM_Init(TIM10, 12000, 10000);
    //ESL_TIM_Init(TIM11, 12000, 65535);

    ESL_TIM_Enable_IRQ(TIM10);

    ESL_NVIC_Set_Priority(TIM1_UP_TIM10_IRQn, 5);
    // Enable global interrupt on timer 10
    ESL_NVIC_Enable_IRQ(TIM1_UP_TIM10_IRQn);
}
