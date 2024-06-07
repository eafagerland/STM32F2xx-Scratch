/********************************************************************************************
 *  Filename: stm32f2xx_esl_systick.c
 *  Author: Erik Fagerland
 *  Created On: 14/02/2024
 *
 *  Brief:
 *  Implementation of systick function.
 *  Handles the systick interrupt with counter, provides a blocking delay function
 *  and a millis function for elapsed uptime.
 *
 *******************************************************************************************/
#include "stm32f2xx_esl_systick.h"
#include "stm32f2xx_esl_pwr.h"

static volatile UInt32 systick_count = 0;

/********************************************************************************************
 *  Initializes the systick, reload value can be set for a custom systick period.
 *  Default at 1ms.
 *******************************************************************************************/
void ESL_SysTick_Init(UInt32 reload_value)
{
    systick_count = 0;

    // Set the reload value
    SYSTICK->STK_LOAD = reload_value - 1U;

    // Init current count value
    SYSTICK->STK_VAL = 0U;

    // Enable Systick, Tickint and set AHB clock
    SET_REG(SYSTICK->STK_CTRL, (SYSTICK_ENABLE | SYSTICK_TICKINT_ENABLE | SYSTICK_CLK_SRC_AHB));
}

/********************************************************************************************
 *  Systick interrupt handler. Increments the systick counter.
 *******************************************************************************************/
void ESL_SysTick_Handler(void)
{
    systick_count++;
}

/********************************************************************************************
 *  Suspends the systick interrupt
 *******************************************************************************************/
void ESL_SysTick_Suspend(void)
{
    RESET_REG(SYSTICK->STK_CTRL, SYSTICK_TICKINT_ENABLE);
}

/********************************************************************************************
 *  Resumes the systick interrupt
 *******************************************************************************************/
void ESL_SysTick_Resume(void)
{
    SET_REG(SYSTICK->STK_CTRL, SYSTICK_TICKINT_ENABLE);
}

/********************************************************************************************
 *  Returns the current systick counter value.
 *******************************************************************************************/
UInt32 ESL_Tick(void)
{
    return systick_count;
}

/********************************************************************************************
 *  Blocking delay function, stays in loop until provided timer value has elapsed.
 *******************************************************************************************/
void ESL_Delay(UInt32 millis)
{
    UInt32 delay_start = systick_count;
    while (systick_count - delay_start <= millis)
    {
        __wfi();
    }
}