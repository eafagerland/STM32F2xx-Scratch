#ifndef __STM32F2xx_ESL_TIMER_H
#define __STM32F2xx_ESL_TIMER_H

#include "stm32f207xx.h"

#define TIM1_BASE   0x40010000U
#define TIM2_BASE   0x40000000U
#define TIM3_BASE   0x40000400U
#define TIM4_BASE   0x40000800U
#define TIM5_BASE   0x40000C00U
#define TIM6_BASE   0x40001000U
#define TIM7_BASE   0x40001400U
#define TIM8_BASE   0x40001400U
#define TIM9_BASE   0x40014000U
#define TIM10_BASE  0x40014400U
#define TIM11_BASE  0x40014800U
#define TIM12_BASE  0x40001800U
#define TIM13_BASE  0x40001C00U
#define TIM14_BASE  0x40002000U

void ESL_TIM_Init(TIMx_Typedef* TIMx, UInt32 psc, UInt32 arr);
void ESL_TIM_Enable_IRQ(TIMx_Typedef* TIMx);
void ESL_TIM_Reset_IRQ(TIMx_Typedef* TIMx);

void TIM10_IRQHandler(void);

#endif