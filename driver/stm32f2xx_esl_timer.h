/********************************************************************************************
 *  Filename: stm32f2xx_esl_timer.h
 *  Author: Erik Fagerland
 *  Created On: 13/02/2024
 * 
 *  Brief:
 *  Header file for timers
 * 
 *******************************************************************************************/
#ifndef __STM32F2xx_ESL_TIMER_H
#define __STM32F2xx_ESL_TIMER_H

#include "stm32f207xx.h"

#define TIM1_BASE   (0x40010000UL)
#define TIM2_BASE   (0x40000000UL)
#define TIM3_BASE   (0x40000400UL)
#define TIM4_BASE   (0x40000800UL)
#define TIM5_BASE   (0x40000C00UL)
#define TIM6_BASE   (0x40001000UL)
#define TIM7_BASE   (0x40001400UL)
#define TIM8_BASE   (0x40001400UL)
#define TIM9_BASE   (0x40014000UL)
#define TIM10_BASE  (0x40014400UL)
#define TIM11_BASE  (0x40014800UL)
#define TIM12_BASE  (0x40001800UL)
#define TIM13_BASE  (0x40001C00UL)
#define TIM14_BASE  (0x40002000UL)

void ESL_TIM_Init(TIMx_TypeDef* tim, UInt32 psc, UInt32 arr);
void ESL_TIM_Enable_IRQ(TIMx_TypeDef* tim);
void ESL_TIM_Reset_IRQ(TIMx_TypeDef* tim);

void ESL_TIM_IRQ_Handler(TIMx_TypeDef* tim);

#endif // __STM32F2xx_ESL_TIMER_H