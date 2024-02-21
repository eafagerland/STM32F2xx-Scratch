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

typedef enum
{
    MONDAY = 1U,
    THUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY,
    SUNDAY
} RTC_Week_Day_TypeDef;

typedef struct
{
    UInt8 second;
    UInt8 minute;
    UInt8 hour;
    UInt8 format;
} ESL_RTC_Time_TypeDef;

typedef struct 
{
    UInt8 date;
    UInt8 month;
    UInt8 year;
    RTC_Week_Day_TypeDef weekday;
} ESL_ETC_Date_TypeDef;


void ESL_RTC_Init(RCC_RTC_Clk_Src_TypeDef clock_source);
ESL_RTC_Time_TypeDef ESL_RTC_Get_Time(void);
ESL_ETC_Date_TypeDef ESL_RTC_Get_Date(void);

#endif // __STM32F2xx_ESL_RTC_H