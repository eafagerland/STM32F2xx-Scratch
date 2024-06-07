/********************************************************************************************
 *  Filename: stm32f2xx_esl_nvic.c
 *  Author: Erik Fagerland
 *  Created On: 13/02/2024
 *
 *  Brief:
 *  Implmentation of NVIC. Provides a function for enabling interrupts and a
 *  weak function on EXTI line 15-10 to be implemented by user.
 *
 *******************************************************************************************/
#include "stm32f2xx_esl_nvic.h"

/********************************************************************************************
 *  Activates interrupt on the provided vector positon. See boot.S for entire vector table.
 *******************************************************************************************/
void ESL_NVIC_Enable_IRQ(UInt8 irq_pos)
{
    // Get the index and position
    UInt8 index = irq_pos / 32;
    UInt8 bitPos = irq_pos % 32;

    // Update NVIC register table to enable the interrupt
    SET_REG(NVIC->ISER[index], (1U << bitPos));
}

/********************************************************************************************
 *  Disables interrupt on the provided vector positon. See boot.S for entire vector table.
 *******************************************************************************************/
void ESL_NVIC_Disable(UInt8 irq_pos)
{
    // Get the index and position
    UInt8 index = irq_pos / 32;
    UInt8 bitPos = irq_pos % 32;

    // Update NVIC register table to disable the interrupt
    SET_REG(NVIC->ICER[index], (1U << bitPos));
}

/********************************************************************************************
 *  Weak declared interrupt handler for EXTI line 15-10.
 *******************************************************************************************/
__weak void EXTI15_10_Handler(void)
{
    /* NOTE:
     * This function should not be modified, when the callback is needed it can be implemented in user file
     */
}