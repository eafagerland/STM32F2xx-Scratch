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
void ESL_NVIC_Enable_IRQ(IRQn_Type irq_pos)
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
void ESL_NVIC_Disable_IRQ(IRQn_Type irq_pos)
{
    // Get the index and position
    UInt8 index = irq_pos / 32;
    UInt8 bitPos = irq_pos % 32;

    // Update NVIC register table to disable the interrupt
    SET_REG(NVIC->ICER[index], (1U << bitPos));
}

/********************************************************************************************
 *  Sets the priority of the interrupt on the provided vector position.
 *******************************************************************************************/
void ESL_NVIC_Set_Priority(IRQn_Type irq_pos, UInt8 priority)
{
    // Ensure priority is within valid range
    if (priority > 255)
        priority = 255;

    // Get the index and shift amount
    UInt8 index = irq_pos / 4;          // Each priority register holds 4 IRQ priorities (8 bits each)
    UInt8 shift = (irq_pos % 4) * 8;    // Calculate the shift amount within the priority register

    // Update NVIC priority register to set the priority
    SET_REG(NVIC->IP[index], ((priority & 0xFF) << shift));
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