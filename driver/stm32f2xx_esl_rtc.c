/********************************************************************************************
 *  Filename: stm32f2xx_esl_rtc.c
 *  Author: Erik Fagerland
 *  Created On: 18/02/2024
 * 
 *  Brief:
 *  Implmenentation file for RTC.
 * 
 *******************************************************************************************/
#include "stm32f2xx_esl_rtc.h"

#define RTC_WRITE_KEY_1         (0xCAU)
#define RTC_WRITE_KEY_2         (0x53U)

#define RTC_ASYNC_PREDIV        (127U)
#define RTC_SYNC_PREDIV         (255U)

#define RTC_PRER_PREDIV_S_POS   (0U)
#define RTC_PRER_PREDIV_A_POS   (16U)

#define RTC_ISR_INITF           (1U << 6U)
#define RTC_ISR_INIT            (1U << 7U)

#define RTC_TR_SEC_LN           (0U)
#define RTC_TR_SEC_HN           (4U)
#define RTC_TR_MIN_LN           (8U)
#define RTC_TR_MIN_HN           (12U)
#define RTC_TR_HOUR_LN          (16U)
#define RTC_TR_HOUR_HN          (20U)
#define RTC_TR_FORMAT           (22U)

#define RTC_24H_FORMAT          (0U)
#define RTC_12H_FORMAT          (1U)

#define RTC_DR_DATE_LN          (0U)
#define RTC_DR_DATE_HN          (4U)
#define RTC_DR_MONTH_LN         (8U)
#define RTC_DR_MONTH_HN         (12U)
#define RTC_DR_YEAR_LN          (16U)
#define RTC_DR_YEAR_HN          (20U)
#define RTC_DR_WEEKDAY          (13U)

static Bool is_rtc_running = FALSE;

/********************************************************************************************
 *  Initializes the RTC with given clock source
 *******************************************************************************************/
void ESL_RTC_Init(RCC_RTC_Clk_Src_TypeDef clock_source)
{
    // Set clock source and enable RTC
    ESL_RCC_RTC_Enable(clock_source);

    // Unlock RTC
    RTC->WPR = RTC_WRITE_KEY_1;
    RTC->WPR = RTC_WRITE_KEY_2;
    (void)RTC->WPR;

    // Set RTC in init mode
    SET_REG(RTC->ISR, RTC_ISR_INIT);
    
    // Wait for init to be active
    while(!IS_BIT_SET(RTC->ISR, RTC_ISR_INITF)){}

    // Set clock frequency
    RESET_REG(RTC->PRER, (0x3FFFFFUL << RTC_PRER_PREDIV_S_POS));
    SET_REG(RTC->PRER, (RTC_SYNC_PREDIV << RTC_PRER_PREDIV_S_POS));
    SET_REG(RTC->PRER, (RTC_ASYNC_PREDIV << RTC_PRER_PREDIV_A_POS));

    // Set hour format
    SET_REG(RTC->TR, (RTC_24H_FORMAT << RTC_TR_FORMAT));

    // TODO: Set date
    // TODO: Set time

    // Set RTC in Free Run mode
    RESET_REG(RTC->ISR, RTC_ISR_INIT);

    // Enable write protection again
    RTC->WPR = 0xFFU;
    (void)RTC->WPR;

    is_rtc_running = TRUE;
}

/********************************************************************************************
 *  Extracts the time values and returns a struct containing all time data
 *******************************************************************************************/
ESL_RTC_Time_TypeDef ESL_RTC_Get_Time(void)
{
    ESL_RTC_Time_TypeDef time = {0};
    if (!is_rtc_running) return time;

    UInt32 reg_tr = RTC->TR;

    // Extract second value
    UInt8 high_nibble = (UInt8)(reg_tr >> RTC_TR_SEC_HN) & 0x07U;
	UInt8 low_nibble = (UInt8)(reg_tr >> RTC_TR_SEC_LN) & 0x0FU;
	time.second = (high_nibble * 10) + low_nibble;

    // Extract minute value
    high_nibble = (UInt8)(reg_tr >> RTC_TR_MIN_HN) & 0x07U;
    low_nibble = (UInt8)(reg_tr >> RTC_TR_MIN_LN) & 0x0FU;
    time.minute = (high_nibble * 10) + low_nibble;

    // Extract hour value
    high_nibble = (UInt8)(reg_tr >> RTC_TR_HOUR_HN) & 0x03U;
    low_nibble = (UInt8)(reg_tr >> RTC_TR_HOUR_LN) & 0x0FU;
    time.hour = (high_nibble * 10) + low_nibble;
    time.format = (UInt8)(reg_tr & RTC_TR_FORMAT);

    return time;
}

/********************************************************************************************
 *  Extracts the date values and returns a struct containing all time data
 *******************************************************************************************/
ESL_ETC_Date_TypeDef ESL_RTC_Get_Date(void)
{
    ESL_ETC_Date_TypeDef date = {0};
    if (!is_rtc_running) return date;

    UInt32 reg_dr = RTC->DR;

    // Extract date
    UInt8 high_nibble = (UInt8)(reg_dr >> RTC_DR_DATE_HN) & 0x3U;
	UInt8 low_nibble = (UInt8)(reg_dr >> RTC_DR_DATE_LN) & 0xFU;
	date.date = (high_nibble * 10) + low_nibble;

    // Extract month
    high_nibble = (UInt8)(reg_dr >> RTC_DR_MONTH_HN) & 0x1U;
    low_nibble = (UInt8)(reg_dr >> RTC_DR_MONTH_LN) & 0xFU;
	date.month = (high_nibble * 10) + low_nibble;

    // Extract year
    high_nibble = (UInt8)(reg_dr >> RTC_DR_YEAR_HN) & 0xFU;
    low_nibble = (UInt8)(reg_dr >> RTC_DR_YEAR_LN) & 0xFU;
	date.year = (high_nibble * 10) + low_nibble;

    // Extract weekday
	date.weekday = (UInt8)(reg_dr >> RTC_DR_WEEKDAY) & 0x7U;

    return date;
}