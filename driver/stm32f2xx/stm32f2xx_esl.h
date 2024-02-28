/********************************************************************************************
 *  Filename: stm32f2xx_esl.h
 *  Author: Erik Fagerland
 *  Created On: 13/02/2024
 * 
 *  Brief:
 *  Header file for STM32f2xx.
 * 
 *******************************************************************************************/
#ifndef __STM32F2xx_ESL_H
#define __STM32F2xx_ESL_H

#include "stm32f2xx_esl_gpio.h"
#include "stm32f2xx_esl_timer.h"
#include "stm32f2xx_esl_nvic.h"
#include "stm32f2xx_esl_rcc.h"
#include "stm32f2xx_esl_systick.h"
#include "stm32f2xx_esl_uart.h"
#include "stm32f2xx_esl_pwr.h"
#include "stm32f2xx_esl_rtc.h"
#include "stm32f2xx_esl_i2c.h"

void ESL_Init();

#endif // __STM32F2xx_ESL_H