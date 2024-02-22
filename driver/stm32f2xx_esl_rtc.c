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
#define RTC_IST_INITS           (1U << 4U)

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
 *  Sets the RTC into or out of init mode
 *******************************************************************************************/
static void set_init_mode_enabled(Bool is_enabled)
{
    if (is_enabled)
    {
        is_rtc_running = FALSE;

        // Unlock RTC
        RTC->WPR = RTC_WRITE_KEY_1;
        RTC->WPR = RTC_WRITE_KEY_2;
        (void)RTC->WPR;

        // Set RTC in init mode
        SET_REG(RTC->ISR, RTC_ISR_INIT);
    
        // Wait for init to be active
        while(!IS_BIT_SET(RTC->ISR, RTC_ISR_INITF)){}
    }
    else
    {
        // Set RTC in Free Run mode
        RESET_REG(RTC->ISR, RTC_ISR_INIT);

        // Enable write protection again
        RTC->WPR = 0xFFU;
        (void)RTC->WPR;

        is_rtc_running = TRUE;
    }
}

/********************************************************************************************
 *  Initializes the RTC with given clock source
 *******************************************************************************************/
void ESL_RTC_Init(RCC_RTC_Clk_Src_TypeDef clock_source)
{
    // Set clock source and enable RTC
    ESL_RCC_RTC_Enable(clock_source);

    set_init_mode_enabled(TRUE);

    // Set clock frequency
    RESET_REG(RTC->PRER, (0x3FFFFFUL << RTC_PRER_PREDIV_S_POS));
    SET_REG(RTC->PRER, (RTC_SYNC_PREDIV << RTC_PRER_PREDIV_S_POS));
    SET_REG(RTC->PRER, (RTC_ASYNC_PREDIV << RTC_PRER_PREDIV_A_POS));

    // Set hour format
    SET_REG(RTC->TR, (RTC_24H_FORMAT << RTC_TR_FORMAT));

    set_init_mode_enabled(FALSE);
}

/********************************************************************************************
 *  Returns TRUE if calender has been reset, will be 0 after complete powerdown when
 *  year is 0.
 *******************************************************************************************/
Bool ESL_RTC_Is_Calender_Init(void)
{
    return IS_BIT_SET(RTC->ISR, RTC_IST_INITS);
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

/********************************************************************************************
 *  Sets the time with given struct, stores in backup domain
 *******************************************************************************************/
void ESL_ETC_Set_Time(ESL_RTC_Time_TypeDef time)
{
    UInt32 temp = RTC->TR;

    set_init_mode_enabled(TRUE);

    // Set format
    RESET_REG(temp, (1U << RTC_TR_FORMAT));
    SET_REG(temp, (time.format & 0x1U) << RTC_TR_FORMAT);

    // Set seconds
    UInt8 tens = time.second / 10U;
    UInt8 units = time.second % 10U;
    UInt8 bcd_tens = tens & 0x7U;
    UInt8 bcd_units = units & 0xFU;

    RESET_REG(temp, (0xFU << RTC_TR_SEC_LN) | (0x7U << RTC_TR_SEC_HN));
    SET_REG(temp, ((bcd_tens << RTC_TR_SEC_HN) | bcd_units << RTC_TR_SEC_LN));

    // Set minutes
    tens = time.minute / 10U;
    units = time.minute % 10U;
    bcd_tens = tens & 0x7U;
    bcd_units = units & 0xFU;

    RESET_REG(temp, (0xFU << RTC_TR_MIN_LN) | (0x7U << RTC_TR_MIN_HN));
    SET_REG(temp, ((bcd_tens << RTC_TR_MIN_HN) | bcd_units << RTC_TR_MIN_LN));

    // Set hours
    tens = time.hour / 10U;
    units = time.hour % 10U;
    bcd_tens = tens & 0x3U;
    bcd_units = units & 0xFU;

    RESET_REG(temp, (0xFU << RTC_TR_HOUR_LN) | (0x3U << RTC_TR_HOUR_HN));
    SET_REG(temp, ((bcd_tens << RTC_TR_HOUR_HN) | bcd_units << RTC_TR_HOUR_LN));

    RTC->TR = temp;
    (void)RTC->TR;

    set_init_mode_enabled(FALSE);
}

/********************************************************************************************
 *  Sets the date with given struct, stores in backup domain
 *******************************************************************************************/
void ESL_ETC_Set_Date(ESL_ETC_Date_TypeDef date)
{
    UInt32 temp = RTC->DR;

    set_init_mode_enabled(TRUE);

    // Set date
    UInt8 tens = date.date / 10U;
    UInt8 units = date.date % 10U;
    UInt8 bcd_tens = tens & 0x3U;
    UInt8 bcd_units = units & 0xFU;

    RESET_REG(temp, (0xFU << RTC_DR_DATE_LN) | (0x3U << RTC_DR_DATE_HN));
    SET_REG(temp, ((bcd_tens << RTC_DR_DATE_HN) | bcd_units << RTC_DR_DATE_LN));

    // Set month
    tens = date.month / 10U;
    units = date.month % 10U;
    bcd_tens = tens & 0x1U;
    bcd_units = units & 0xFU;

    RESET_REG(temp, (0xFU << RTC_DR_MONTH_LN) | (0x1U << RTC_DR_MONTH_HN));
    SET_REG(temp, ((bcd_tens << RTC_DR_MONTH_HN) | bcd_units << RTC_DR_MONTH_LN));

    // Set year
    tens = date.year / 10U;
    units = date.year % 10U;
    bcd_tens = tens & 0xFU;
    bcd_units = units & 0xFU;

    RESET_REG(temp, (0xFU << RTC_DR_YEAR_LN) | (0xFU << RTC_DR_YEAR_HN));
    SET_REG(temp, ((bcd_tens << RTC_DR_YEAR_HN) | bcd_units << RTC_DR_YEAR_LN));

    // Set weekday
    RESET_REG(temp, (0x7U << RTC_DR_WEEKDAY));
    SET_REG(temp, ((date.weekday & 0x7U) << RTC_DR_WEEKDAY));

    RTC->DR = temp;
    (void)RTC->DR;

    set_init_mode_enabled(FALSE);
}