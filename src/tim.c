#include "tim.h"
#include "stm32f2xx_esl_timer.h"

void TIM_Init(void)
{
    ESL_TIM_Init(TIM10, 12000, 10000);
    ESL_TIM_Init(TIM11, 12000, 65535);

    ESL_TIM_Enable_IRQ(TIM10);
}
