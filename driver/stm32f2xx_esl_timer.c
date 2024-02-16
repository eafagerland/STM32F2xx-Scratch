/********************************************************************************************
 *  Filename: stm32f2xx_esl_timer.c
 *  Author: Erik Fagerland
 *  Created On: 13/02/2024
 * 
 *  Brief:
 *  Implementation of timer functions on the MCU.
 * 
 *******************************************************************************************/
#include "stm32f2xx_esl_timer.h"

/********************************************************************************************
 *  Initializes and starts the timer with given prescaler and auto reload register 
 *  value.
 *******************************************************************************************/
void ESL_TIM_Init(TIMx_Typedef* TIMx, UInt32 psc, UInt32 arr)
{
    TIMx->PSC = (psc - 1);  // Set the prescaler
    TIMx->ARR = (arr - 1);  // Set the auto reload register
    TIMx->CR1 = 1U;         // enable counter
}

/********************************************************************************************
 *  Enables interrupt on the timer so a interrupt is triggered on each auto reload overflow.
 *******************************************************************************************/
void ESL_TIM_Enable_IRQ(TIMx_Typedef* TIMx)
{
    TIMx->DIER = 1U; // Enable update interrupt
}

/********************************************************************************************
 *  Resets interrupt on given timer, must be called in order for timers interrupt to be 
 *  called again.
 *******************************************************************************************/
void ESL_TIM_Reset_IRQ(TIMx_Typedef* TIMx)
{
    TIM10->SR = 0U;
}

/********************************************************************************************
 *  Interrupt handler for Timer 10, passed into a IRQ handler for all timers for easier
 *  management from user.
 *******************************************************************************************/
void TIM10_IRQHandler(void)
{
    ESL_TIM_IRQ_Handler(TIM10);
}

/********************************************************************************************
 *  weak declared function of the timer interrupt handler to be implmeneted by user.
 *******************************************************************************************/
__weak void ESL_TIM_IRQ_Handler(TIMx_Typedef* TIMx)
{
    /* NOTE: 
     * This function should not be modified, when the callback is needed it can be implemented in user file
    */
}