#include "stm32f2xx_esl_timer.h"

void ESL_TIM_Init(TIMx_Typedef* TIMx, UInt32 psc, UInt32 arr)
{
    TIMx->PSC = psc;	    // Set the prescaler
	TIMx->ARR = arr;        // Set the auto reload register
	TIMx->CR1 = 1U;         // enable counter
}

void ESL_TIM_Enable_IRQ(TIMx_Typedef* TIMx)
{
    TIMx->DIER = 1U; // Enable update interrupt
}

__attribute__((weak)) void TIM10_IRQHandler(void)
{
    /* NOTE: 
     * This function should not be modified, when the callback is needed it can be implemented in user file
    */
}