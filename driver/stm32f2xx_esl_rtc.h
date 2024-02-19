/********************************************************************************************
 *  Filename: stm32f2xx_esl_rtc.h
 *  Author: Erik Fagerland
 *  Created On: 18/02/2024
 * 
 *  Brief:
 *  Header file for RTC.
 * 
 *******************************************************************************************/
#ifndef __STM32F2xx_ESL_RTC_H
#define __STM32F2xx_ESL_RTC_H

#include "stm32f207xx.h"
#include "stm32f2xx_esl_rcc.h"

void ESL_RTC_Init(RCC_RTC_Clk_Src_TypeDef clock_source);

#endif // __STM32F2xx_ESL_RTC_H