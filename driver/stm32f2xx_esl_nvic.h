#ifndef __STM32F2xx_ESL_NVIC_H
#define __STM32F2xx_ESL_NVIC_H

#include "stm32f207xx.h"

#define NVIC_BASE 0xE000E100U

#define NVIC_TIM10_IRQ_POS 25

void NVIC_Enable(UInt8 irq_pos);

#endif // __STM32F2xx_ESL_NVIC_H