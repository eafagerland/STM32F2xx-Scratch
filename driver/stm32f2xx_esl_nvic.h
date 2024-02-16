/********************************************************************************************
 *  Filename: stm32f2xx_esl_nvic.h
 *  Author: Erik Fagerland
 *  Created On: 13/02/2024
 * 
 *  Brief:
 *  Header file of NVIC.
 * 
 *******************************************************************************************/
#ifndef __STM32F2xx_ESL_NVIC_H
#define __STM32F2xx_ESL_NVIC_H

#include "stm32f207xx.h"

#define NVIC_BASE 0xE000E100U

#define NVIC_TIM10_IRQ_POS 25

void ESL_NVIC_Enable(UInt8 irq_pos);

void EXTI15_10_Handler(void);

#endif // __STM32F2xx_ESL_NVIC_H