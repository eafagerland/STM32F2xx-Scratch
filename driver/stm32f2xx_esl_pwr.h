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

extern UInt8 g_pwr_stop_mode_active;

void __wfi(void);
void ESL_Enter_PWR_Stop_Mode(void);
void ESL_Enter_PWR_Standby_Mode(void);

#endif // __STM32F2xx_ESL_PWR_H