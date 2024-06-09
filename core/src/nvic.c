#include "nvic.h"
#include "stm32f2xx_esl_nvic.h"
#include "stm32f2xx_esl_systick.h"

void NVIC_Init(void)
{
    // Enable SysTick
    ESL_SysTick_Init(120000); // 120mhz clock / 120000 to get tick every 1ms
}