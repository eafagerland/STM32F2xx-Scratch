/********************************************************************************************
 *  Filename: stm32f2xx_esl_gpio.h
 *  Author: Erik Fagerland
 *  Created On: 13/02/2024
 * 
 *  Brief:
 *  Header file of GPIO
 * 
 *******************************************************************************************/
#ifndef __STM32F2xx_ESL_GPIO_H
#define __STM32F2xx_ESL_GPIO_H

#include "stm32f207xx.h"

#define GPIOA_BASE				(0x40020000UL)
#define GPIOB_BASE				(0x40020400UL)
#define GPIOC_BASE				(0x40020800UL)
#define GPIOD_BASE				(0x40020C00UL)
#define GPIOE_BASE				(0x40021000UL)

#define GPIO_MODE_Pos				0U
#define GPIO_MODE				(0x3UL << GPIO_MODE_Pos)
#define MODE_INPUT				(0x0UL << GPIO_MODE_Pos)
#define MODE_OUTPUT				(0x1UL << GPIO_MODE_Pos)
#define MODE_AF					(0x2UL << GPIO_MODE_Pos)
#define MODE_ANALOG				(0x3U << GPIO_MODE_Pos)
#define OUTPUT_TYPE_Pos				4U
#define OUTPUT_TYPE				(0x1UL << OUTPUT_TYPE_Pos)
#define OUTPUT_PP				(0x0UL << OUTPUT_TYPE_Pos)
#define OUTPUT_OD				(0x1UL << OUTPUT_TYPE_Pos)

#define GPIO_MODE_INPUT				MODE_INPUT
#define GPIO_MODE_OUTPUT_PP			(MODE_OUTPUT | OUTPUT_PP) 
#define GPIO_MODE_OUTPUT_OD			(MODE_OUTPUT | OUTPUT_OD) 
#define GPIO_MODE_AF_PP				(MODE_AF | OUTPUT_PP)                                 
#define GPIO_MODE_AF_OD				(MODE_AF | OUTPUT_OD) 

#define EXTI_MODE_Pos				16U
#define EXTI_MODE				(0x3UL << EXTI_MODE_Pos)
#define EXTI_IT					(0x1UL << EXTI_MODE_Pos)
#define EXTI_EVT				(0x2UL << EXTI_MODE_Pos)
#define TRIGGER_MODE_Pos			20U
#define TRIGGER_MODE				(0x7UL << TRIGGER_MODE_Pos)
#define TRIGGER_RISING				(0x1UL << TRIGGER_MODE_Pos)
#define TRIGGER_FALLING				(0x2UL << TRIGGER_MODE_Pos)

#define  GPIO_MODE_IT_RISING			(MODE_INPUT | EXTI_IT | TRIGGER_RISING)                    
#define  GPIO_MODE_IT_FALLING			(MODE_INPUT | EXTI_IT | TRIGGER_FALLING)
#define  GPIO_MODE_IT_RISING_FALLING		(MODE_INPUT | EXTI_IT | TRIGGER_RISING | TRIGGER_FALLING)

#define GPIO_AF7_USART1				(0x07UL)
#define GPIO_AF7_USART2				(0x07UL)
#define GPIO_AF7_USART3				(0x07UL)

#define GPIO_PIN_0				((UInt16)0x0001) 
#define GPIO_PIN_1				((UInt16)0x0002) 
#define GPIO_PIN_2				((UInt16)0x0004) 
#define GPIO_PIN_3				((UInt16)0x0008)
#define GPIO_PIN_4				((UInt16)0x0010) 
#define GPIO_PIN_5				((UInt16)0x0020) 
#define GPIO_PIN_6				((UInt16)0x0040) 
#define GPIO_PIN_7				((UInt16)0x0080) 
#define GPIO_PIN_8				((UInt16)0x0100) 
#define GPIO_PIN_9				((UInt16)0x0200) 
#define GPIO_PIN_10				((UInt16)0x0400) 
#define GPIO_PIN_11				((UInt16)0x0800) 
#define GPIO_PIN_12				((UInt16)0x1000) 
#define GPIO_PIN_13				((UInt16)0x2000) 
#define GPIO_PIN_14				((UInt16)0x4000) 
#define GPIO_PIN_15				((UInt16)0x8000) 
#define GPIO_PIN_All				((UInt16)0xFFFF)

// Macro for checking EXTI source on given pin, return 1 if set.
#define GPIO_EXTI_SOURCE(x) EXTI->PR & x

typedef struct
{
	UInt32 Pin;
	UInt32 Mode;
	UInt32 Pull;
	UInt32 Speed;
	UInt32 Alternate;
} GPIO_InitTypeDef;

typedef enum
{
	GPIO_PIN_RESET = 0U,
	GPIO_PIN_SET
}GPIO_PinState;

typedef enum
{
	PORT_GPIOA = 0U,
	PORT_GPIOB,
	PORT_GPIOC,
	PORT_GPIOD,
	PORT_GPIOE
} GPIO_Port_Typedef;

void ESL_GPIO_Init(GPIO_Typedef* GPIOx, GPIO_InitTypeDef* GPIO_Init);
void ESL_GPIO_WritePin(GPIO_Typedef* GPIOx, UInt16 GPIO_Pin, GPIO_PinState PinState);
void ESL_GPIO_TogglePin(GPIO_Typedef* GPIOx, UInt16 GPIO_Pin);
GPIO_PinState ESL_GPIO_Read_Pinstate(GPIO_Typedef* GPIOx, UInt16 GPIO_Pin);

#endif // __STM32F2xx_ESL_GPIO_H