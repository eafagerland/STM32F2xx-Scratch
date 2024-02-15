#ifndef __STM32F2xx_ESL_GPIO_H
#define __STM32F2xx_ESL_GPIO_H

#include "stm32f207xx.h"

#define GPIOA_BASE				0x40020000U
#define GPIOB_BASE				0x40020400U
#define GPIOC_BASE				0x40020800U
#define GPIOD_BASE				0x40020C00U
#define GPIOE_BASE				0x40021000U

#define GPIO_MODE_Pos			0U
#define MODE_INPUT				(0x0UL << GPIO_MODE_Pos)
#define MODE_OUTPUT				(0x1UL << GPIO_MODE_Pos)
#define MODE_AF					(0x2UL << GPIO_MODE_Pos)
#define MODE_ANALOG				(0x3U << GPIO_MODE_Pos)
#define OUTPUT_TYPE_Pos			4U
#define OUTPUT_TYPE				(0x1UL << OUTPUT_TYPE_Pos)
#define OUTPUT_PP				(0x0UL << OUTPUT_TYPE_Pos)
#define OUTPUT_OD				(0x1UL << OUTPUT_TYPE_Pos)

#define GPIO_MODE_INPUT			MODE_INPUT
#define GPIO_MODE_OUTPUT_PP		(MODE_OUTPUT | OUTPUT_PP) 
#define GPIO_MODE_OUTPUT_OD		(MODE_OUTPUT | OUTPUT_OD) 
#define GPIO_MODE_AF_PP			(MODE_AF | OUTPUT_PP)                                 
#define GPIO_MODE_AF_OD			(MODE_AF | OUTPUT_OD) 

#define EXTI_MODE_Pos			16U
#define EXTI_MODE				(0x3UL << EXTI_MODE_Pos)
#define EXTI_IT					(0x1UL << EXTI_MODE_Pos)
#define EXTI_EVT				(0x2UL << EXTI_MODE_Pos)
#define TRIGGER_MODE_Pos		20U
#define TRIGGER_MODE			(0x7UL << TRIGGER_MODE_Pos)
#define TRIGGER_RISING			(0x1UL << TRIGGER_MODE_Pos)
#define TRIGGER_FALLING			(0x2UL << TRIGGER_MODE_Pos)

#define  GPIO_MODE_IT_RISING			(MODE_INPUT | EXTI_IT | TRIGGER_RISING)                    
#define  GPIO_MODE_IT_FALLING			(MODE_INPUT | EXTI_IT | TRIGGER_FALLING)
#define  GPIO_MODE_IT_RISING_FALLING	(MODE_INPUT | EXTI_IT | TRIGGER_RISING | TRIGGER_FALLING)

#define GPIO_PIN_0				((UInt16)0x0001)  /* Pin 0 selected    */
#define GPIO_PIN_1				((UInt16)0x0002)  /* Pin 1 selected    */
#define GPIO_PIN_2				((UInt16)0x0004)  /* Pin 2 selected    */
#define GPIO_PIN_3				((UInt16)0x0008)  /* Pin 3 selected    */
#define GPIO_PIN_4				((UInt16)0x0010)  /* Pin 4 selected    */
#define GPIO_PIN_5				((UInt16)0x0020)  /* Pin 5 selected    */
#define GPIO_PIN_6				((UInt16)0x0040)  /* Pin 6 selected    */
#define GPIO_PIN_7				((UInt16)0x0080)  /* Pin 7 selected    */
#define GPIO_PIN_8				((UInt16)0x0100)  /* Pin 8 selected    */
#define GPIO_PIN_9				((UInt16)0x0200)  /* Pin 9 selected    */
#define GPIO_PIN_10				((UInt16)0x0400)  /* Pin 10 selected   */
#define GPIO_PIN_11				((UInt16)0x0800)  /* Pin 11 selected   */
#define GPIO_PIN_12				((UInt16)0x1000)  /* Pin 12 selected   */
#define GPIO_PIN_13				((UInt16)0x2000)  /* Pin 13 selected   */
#define GPIO_PIN_14				((UInt16)0x4000)  /* Pin 14 selected   */
#define GPIO_PIN_15				((UInt16)0x8000)  /* Pin 15 selected   */
#define GPIO_PIN_All			((UInt16)0xFFFF)  /* All pins selected */

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

void ESL_GPIO_Init(GPIO_Typedef* GPIOx, GPIO_InitTypeDef* GPIO_Init);
void ESL_GPIO_WritePin(GPIO_Typedef* GPIOx, UInt16 GPIO_Pin, GPIO_PinState PinState);
void ESL_GPIO_TogglePin(GPIO_Typedef* GPIOx, UInt16 GPIO_Pin);
GPIO_PinState ESL_GPIO_Read_Pinstate(GPIO_Typedef* GPIOx, UInt16 GPIO_Pin);

#endif // __STM32F2xx_ESL_GPIO_H