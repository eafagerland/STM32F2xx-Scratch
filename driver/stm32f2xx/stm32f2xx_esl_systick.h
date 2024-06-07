/********************************************************************************************
 *  Filename: stm32f2xx_esl_systick.h
 *  Author: Erik Fagerland
 *  Created On: 14/02/2024
 *
 *  Brief:
 *  Header file for systick.
 *
 *******************************************************************************************/
#ifndef __STM32F2xx_ESL_SYSTICK_H
#define __STM32F2xx_ESL_SYSTICK_H

#include "stm32f207xx.h"

#define SYSTICK_BASE            (0xE000E010UL)

#define SYSTICK_ENABLE          (1U << 0U)
#define SYSTICK_TICKINT_ENABLE  (1U << 1U)
#define SYSTICK_CLK_SRC_AHB     (1U << 2U)
#define SYSTICK_CLK_SRC_AHB8    (1U << 2U)

void ESL_SysTick_Init(UInt32 reload_value);
void ESL_SysTick_Handler(void);
void ESL_SysTick_Suspend(void);
void ESL_SysTick_Resume(void);

UInt32 ESL_Tick(void);
void ESL_Delay(UInt32 millis);

#endif // __STM32F2xx_ESL_SYSTICK_H