#include "stm32f2xx_esl_systick.h"

#define SYSTICK_ENABLE              (1 << 0)
#define SYSTICK_TICKINT_ENABLE      (1 << 1)
#define SYSTICK_CLK_SRC_AHB         (1 << 2)
#define SYSTICK_CLK_SRC_AHB8        (1 << 2)

static volatile UInt32 systick_count = 0;
static volatile UInt32 delay_start_millis = 0;

void ESL_SysTick_Init(UInt32 reload_value)
{
    systick_count = 0;

    // Set the reload value
    SYSTICK->STK_LOAD = reload_value - 1U;

    // Init current count value
    SYSTICK->STK_VAL = 0U;

    // Enable Systick, Tickint and set AHB clock
    SYSTICK->STK_CTRL |= SYSTICK_ENABLE | SYSTICK_TICKINT_ENABLE | SYSTICK_CLK_SRC_AHB;
}

void ESL_SysTick_Handler(void)
{
    systick_count++;
}

UInt32 ESL_Millis(void)
{
    return systick_count;
}

void ESL_Delay(UInt32 millis)
{
    delay_start_millis = systick_count;
    while (systick_count - delay_start_millis <= millis)
    {
    }
}