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
#define SCB_SCR_SLEEPDEEP		(1U << 2U)

// Power Register
#define PWR_CR_LPDS			(1U << 0U) // Low-power deep sleep
#define PWR_CR_PDDS			(1U << 1U) // Power down deepsleep

Bool g_pwr_stop_mode_active = FALSE;

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
void ESL_PWR_Enter_Sleep(PWR_SLP_PDDS_Typedef sleep_mode, PWR_SLP_LPDS_Typedef regulator_state)
{
	// Disable systick
	ESL_SysTick_Suspend();

	// Reset PDDS to Stop Mode
	RESET_REG(PWR->CR, PWR_CR_PDDS);

	// Set to standby if enabled
	if (sleep_mode == PWR_SLP_PPDS_STB)
		SET_REG(PWR->CR, PWR_CR_PDDS);

	// Reset voltage regulators to ON
	RESET_REG(PWR->CR, PWR_CR_LPDS);

	// Regulators to low-power if set
	if (regulator_state == PWR_SLP_LPDS_OFF)
		SET_REG(PWR->CR, PWR_CR_LPDS);

	// Reset and set SLEEPDEEP bit for MCU to enter deep sleep on __wfi()
	RESET_REG(SCB->SCR, SCB_SCR_SLEEPDEEP);
	SET_REG(SCB->SCR, SCB_SCR_SLEEPDEEP);

	DBGMCU->CR = 0; // Deactivate debug trace during sleep

	g_pwr_stop_mode_active = TRUE; // This cant be used if standbymode is selected, as ram is cleared
	__wfi(); // Enter Stop Mode

	RESET_REG(SCB->SCR, SCB_SCR_SLEEPDEEP);	// Reset SLEEPDEEP bit
}
