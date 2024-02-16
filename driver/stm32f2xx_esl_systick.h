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

#define SYSTICK_BASE 0xE000E010U

void ESL_SysTick_Init(UInt32 reload_value);
void ESL_SysTick_Handler(void);

UInt32 ESL_Millis(void);
void ESL_Delay(UInt32 millis);

#endif // __STM32F2xx_ESL_SYSTICK_H