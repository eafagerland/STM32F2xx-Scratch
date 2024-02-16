/********************************************************************************************
 *  Filename: stm32f2xx_esl_pwr.c
 *  Author: Erik Fagerland
 *  Created On: 16/02/2024
 * 
 *  Brief:
 *  Implementation of power settings on the MCU
 * 
 *******************************************************************************************/
#include "stm32f2xx_esl_pwr.h"
#include "stm32f2xx_esl_systick.h"
#include "stm32f2xx_esl_rcc.h"
#include "stm32f2xx_esl_nvic.h"
#include "stm32f2xx_esl_timer.h"
#include "stm32f207xx.h"

// SCB Register
#define SCB_SCR_SLEEPDEEP (1 << 2)

// Power Register
#define PWR_CR_LPDS (1 << 0) // Low-power deep sleep
#define PWR_CR_PDDS (1 << 1) // Power down deepsleep

UInt8 g_pwr_stop_mode_active = 0U;

/********************************************************************************************
 *  Calls the wait for interrupt instruction
 *******************************************************************************************/
void __wfi(void)
{
    __asm volatile ("wfi");
}

/********************************************************************************************
 *  Enters stop mode, 1.2V domain off, HSI and HSE oscillators off. Wake-up with EXTI lines
 *******************************************************************************************/
void ESL_Enter_PWR_Stop_Mode(void)
{
	// Disable systick
	ESL_SysTick_Suspend();

	// Reset PDDS for stop mode
	PWR->CR &= ~PWR_CR_PDDS;

	// Disable voltage regulators
	PWR->CR &= ~PWR_CR_LPDS; 
	PWR->CR |= PWR_CR_LPDS;

	SCB->SCR &= ~SCB_SCR_SLEEPDEEP; // Reset SLEEPDEEP bit
	SCB->SCR |= SCB_SCR_SLEEPDEEP;	// Set SLEEPDEEP bit

	DBGMCU->CR = 0; // Deactivate debug during sleep

	g_pwr_stop_mode_active = 1U;
	__wfi();

	SCB->SCR &= ~SCB_SCR_SLEEPDEEP; // Reset SLEEPDEEP bit
}

/********************************************************************************************
 *  Enters standby mode, 1.2V domain off, HSI and HSE oscillators off and voltage regulators
 *  off. Wake-up with dedicated wake-up pin or RTC
 *******************************************************************************************/
void ESL_Enter_PWR_Standby_Mode(void)
{
	// TODO: Implement
}
