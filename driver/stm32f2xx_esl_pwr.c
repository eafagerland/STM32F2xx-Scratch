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
#define PWR_CR_LPDS			(1U << 0U) 	// Low-power deep sleep
#define PWR_CR_PDDS			(1U << 1U) 	// Power down deepsleep
#define PWR_CR_CWUF			(1U << 2U)	// Clear wakeup flag
#define PWR_CR_CSBF			(1U << 3U)	// Clear standby flag

#define PWR_CSR_EWUP		(1U << 8U) 	// Enable wake-up pin
#define PWR_CSR_WUF			(1U << 0U)	// Wakeup flag 
#define PWR_CSR_SBF			(1U << 1U) 	// Standby flag

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

	// Clear wake up and standby flag
	SET_REG(PWR->CR, (PWR_CR_CWUF | PWR_CR_CSBF));

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

	(void)PWR->CR; // Ensure that the previous PWR register operations have been completed

	// Reset and set SLEEPDEEP bit for MCU to enter deep sleep on __wfi()
	RESET_REG(SCB->SCR, SCB_SCR_SLEEPDEEP);
	SET_REG(SCB->SCR, SCB_SCR_SLEEPDEEP);

	(void)SCB->SCR; // Ensure that the previous PWR register operations have been completed

	DBGMCU->CR = 0; // Deactivate debug trace during sleep

	g_pwr_stop_mode_active = TRUE; // This cant be used if standbymode is selected, as ram is cleared
	__wfi(); // Enter Stop Mode

	RESET_REG(SCB->SCR, SCB_SCR_SLEEPDEEP);	// Reset SLEEPDEEP bit
}

/********************************************************************************************
 *  Enables the PA0 (STM32F207) as wake-up pin. (Pull-down is enabled)
 * 	NOTE: On NUCLEO-F207ZG SB180 needs to be soldered closed to connect it to USER button
 *******************************************************************************************/
void ESL_PWR_Enable_WKUP_Pin(void)
{
	RESET_REG(PWR->CSR, PWR_CSR_EWUP);
	SET_REG(PWR->CSR, PWR_CSR_EWUP);
}

/********************************************************************************************
 *  This can be called to check if MCU woke up from a standby mode
 *******************************************************************************************/
Bool ESL_PWR_Standby_Flagged(void)
{
	Bool standby_flagged = FALSE;

	// Check if standby flag is set, indicating it woke up from standby
	if (IS_BIT_SET(PWR->CSR, PWR_CSR_SBF)) 
		standby_flagged = TRUE;

	// Clear standby flag
	SET_REG(PWR->CR, PWR_CR_CSBF);

	return standby_flagged;
}

/********************************************************************************************
 *  This can be called to check if MCU woke up from a wakeup event
 *******************************************************************************************/
Bool ESL_PWR_WKUP_Flagged(void)
{
	Bool wakup_flagged = FALSE;

	// Check if standby flag is set, indicating it woke up from standby
	if (IS_BIT_SET(PWR->CSR, PWR_CSR_WUF)) 
		wakup_flagged = TRUE;

	// Clear wakup flag
	SET_REG(PWR->CR, PWR_CR_CWUF);

	return wakup_flagged;
}