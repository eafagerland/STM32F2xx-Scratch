#include "stm32f2xx_esl_timer.h"

void ESL_TIM_Init(TIMx_Typedef* TIMx, UInt32 psc, UInt32 arr)
{
    TIMx->PSC = (psc - 1);  // Set the prescaler
    TIMx->ARR = (arr - 1);  // Set the auto reload register
    TIMx->CR1 = 1U;         // enable counter
}

void ESL_TIM_Enable_IRQ(TIMx_Typedef* TIMx)
{
    TIMx->DIER = 1U; // Enable update interrupt
}

void ESL_TIM_Reset_IRQ(TIMx_Typedef* TIMx)
{
    TIM10->SR = 0U;
}

void TIM10_IRQHandler(void)
{
    ESL_TIM_IRQ_Handler(TIM10);
}

__attribute__((weak)) void ESL_TIM_IRQ_Handler(TIMx_Typedef* TIMx)
{
    /* NOTE: 
     * This function should not be modified, when the callback is needed it can be implemented in user file
    */
}