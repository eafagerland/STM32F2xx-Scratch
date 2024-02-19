#include "rtc.h"
#include "stm32f2xx_esl_rtc.h"

void RTC_Init()
{
    ESL_RTC_Init(RTC_CLK_LSE);
}