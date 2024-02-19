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

#define RTC_WRITE_KEY_1     (0xCAU)
#define RTC_WRITE_KEY_2     (0x53U)

#define RTC_ASYNC_PREDIV    (127U)
#define RTC_SYNC_PREDIV     (255U)

#define RTC_PRER_PREDIV_S_POS   (0U)
#define RTC_PRER_PREDIV_A_POS   (16U)

#define RTC_ISR_INITF       (1U << 6U)
#define RTC_ISR_INIT        (1U << 7U)

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

    // TODO: Set date
    // TODO: Set time
    // TODO: Set hour format

    RESET_REG(RTC->ISR, RTC_ISR_INIT);
}