/********************************************************************************************
 *  Filename: stm32f2xx_esl_i2c.h
 *  Author: Erik Fagerland
 *  Created On: 24/02/2024
 *
 *  Brief:
 *  Header file of I2C
 *
 *******************************************************************************************/
#ifndef __STM32F2xx_ESL_I2C_H
#define __STM32F2xx_ESL_I2C_H

#include "stm32f207xx.h"

ESL_StatusTypeDef ESL_I2C_Init(I2C_TypeDef *i2c);
ESL_StatusTypeDef ESL_I2C_Master_Transmit(I2C_TypeDef *i2c, UInt8 address, UInt8 *buf, UInt32 len);
ESL_StatusTypeDef ESL_I2C_Master_Receive(I2C_TypeDef *i2c, UInt8 address, UInt8 *buf, UInt32 len);

#endif // __STM32F2xx_ESL_I2C_H