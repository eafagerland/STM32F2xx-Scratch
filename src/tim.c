#include "tim.h"
#include "stm32f2xx_esl_timer.h"

void TIM_Init(void)
{
    ESL_TIM_Init(TIM10, 1600 - 1, 10000 - 1);
    ESL_TIM_Init(TIM11, 1600 - 1, 65535 - 1);

    ESL_TIM_Enable_IRQ(TIM10);
}