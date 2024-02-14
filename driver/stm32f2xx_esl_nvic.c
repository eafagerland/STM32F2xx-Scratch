#include "stm32f2xx_esl_nvic.h"

void ESL_NVIC_Enable(UInt8 irq_pos)
{
    // Get the index and position
    UInt8 index = irq_pos / 32;
    UInt8 bitPos = irq_pos % 32;

    // Update NVIC register table to enable the interrupt
    NVIC->ISER[index] = (1 << bitPos);
}