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

#define SCS_BASE		(0xE000E000UL)
#define SCB_BASE        (SCS_BASE +  0x0D00UL)
#define PWR_BASE		0x40007000U

#define SCB             ((SCB_Typedef* ) SCB_BASE)
#define PWR				((PWR_Typedef* ) PWR_BASE)

// SCB Register
#define SCB_SCR_SLEEPDEEP (1 << 2)

// Power Register
#define PWR_CR_LPDS (1 << 0) // Low-power deep sleep
#define PWR_CR_PDDS (1 << 1) // Power down deepsleep

// System Control Block Typedef
typedef struct
{
	UInt32 CPUID;               
	UInt32 ICSR;                
	UInt32 VTOR;                
	UInt32 AIRCR;                
	UInt32 SCR; 
	UInt32 CCR;                   
	UInt32 SHP[12U];             
	UInt32 SHCSR;                 
	UInt32 CFSR;                  
	UInt32 HFSR;                  
	UInt32 DFSR;                  
	UInt32 MMFAR;                 
	UInt32 BFAR;                  
	UInt32 AFSR;                  
	UInt32 PFR[2U];               
	UInt32 DFR;                   
	UInt32 ADR;                   
	UInt32 MMFR[4U];              
	UInt32 ISAR[5U];              
	UInt32 RESERVED0[5U];
	UInt32 CPACR;                 
} SCB_Typedef;

// Power Register Typedef
typedef struct
{
	UInt32 CR;
	UInt32 CSR;
} PWR_Typedef;

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

	volatile UInt32 pwrReg = PWR->CR;
	volatile UInt32 scrReg = SCB->SCR;

	__wfi();

	SCB->SCR &= ~SCB_SCR_SLEEPDEEP; // Reset SLEEPDEEP bit
}

/********************************************************************************************
 *  Enters standby mode, 1.2V domain off, HSI and HSE oscillators off and voltage regulators
 *  off. Wake-up with dedicated wake-up pin or RTC
 *******************************************************************************************/
void ESL_Enter_PWR_Standby_Mode(void)
{

}
