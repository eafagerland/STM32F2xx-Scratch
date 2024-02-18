/********************************************************************************************
 *  Filename: stm32f2xx_esl_pwr.h
 *  Author: Erik Fagerland
 *  Created On: 16/02/2024
 * 
 *  Brief:
 *  Header file of Power function on the MCU
 * 
 *******************************************************************************************/
#ifndef __STM32F2xx_ESL_PWR_H
#define __STM32F2xx_ESL_PWR_H

#include "stm32f207xx.h"

/********************************************************************************************
 *  When entering sleep, low voltage regulators can be set to low-power to save power.
 *  Given as argument when entering sleep mode to keep enabled or not.
 *******************************************************************************************/
typedef enum
{
    PWR_SLP_LPDS_ON = 0U,
    PWR_SLP_LPDS_OFF
} PWR_SLP_LPDS_TypeDef;

/********************************************************************************************
 *  When entering sleep mode user can select which sleep mode to enter.
 *  Standby mode or stop mode. 
 *******************************************************************************************/
typedef enum
{
    PWR_SLP_PPDS_STOP = 0U,
    PWR_SLP_PPDS_STB
} PWR_SLP_PDDS_TypeDef;

extern Bool g_pwr_stop_mode_active;

void __wfi(void);
void ESL_PWR_Enter_Sleep(PWR_SLP_PDDS_TypeDef sleep_mode, PWR_SLP_LPDS_TypeDef regulator_state);
void ESL_PWR_Enable_WKUP_Pin(void);
Bool ESL_PWR_Standby_Flagged(void);
Bool ESL_PWR_WKUP_Flagged(void);

#endif // __STM32F2xx_ESL_PWR_H