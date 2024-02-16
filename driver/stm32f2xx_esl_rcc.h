/***************************************************************************
 *  Filename: stm32f2xx_esl_rcc.h
 *  Author: Erik Fagerland
 *  Created On: 14/02/2024
 * 
 *  Brief:
 *  Header file of RCC
 * 
 **************************************************************************/
#ifndef __STM32F2xx_ESL_RCC_H
#define __STM32F2xx_ESL_RCC_H

#include "stm32f207xx.h"

/* 
 * Target sysclock of MCU. PLL, AHB and APB settings must be recalculated
 * in order to change clock! It's not enough to just change this definition! 
 */
#define RCC_SYSCLK_TARGET 120000000

#define RCC_HSE_CRYSTAL_FREQ 8000000 // Frequency on the onboard HSE crystal

#define RCC_BASE 0x40023800U

#define FLASH_ACR_LATENCY_POS 0U

// Flash interface latency write states
typedef enum
{
    FLASH_LATENCY_0WS = 0U,
    FLASH_LATENCY_1WS,
    FLASH_LATENCY_2WS,
    FLASH_LATENCY_3WS,
    FLASH_LATENCY_4WS,
    FLASH_LATENCY_5WS,
    FLASH_LATENCY_6WS,
    FLASH_LATENCY_7WS
} FLASH_LATENCY_Typedef;

// PLLP Dividers
typedef enum
{
    RCC_PLLP_CLOCK_DIV2 = 0U,
    RCC_PLLP_CLOCK_DIV4,
    RCC_PLLP_CLOCK_DIV6,
    RCC_PLLP_CLOCK_DIV8
} RCC_PLLP_DIV;

// APB Dividers
typedef enum
{
    RCC_APBx_CLOCK_DIV1 = 0U,
    RCC_APBx_CLOCK_DIV2 = 4U,
    RCC_APBx_CLOCK_DIV4 = 5U,
    RCC_APBx_CLOCK_DIV8 = 6U,
    RCC_APBx_CLOCK_DIV16 = 7U
} RCC_APB_DIV;

// AHB Dividers
typedef enum
{
    RCC_AHB_CLOCK_DIV1 = 0U,
    RCC_AHB_CLOCK_DIV2 = 8U,
    RCC_AHB_CLOCK_DIV4 = 9U,
    RCC_AHB_CLOCK_DIV8 = 10U,
    RCC_AHB_CLOCK_DIV16 = 11U,
    RCC_AHB_CLOCK_DIV64 = 12U,
    RCC_AHB_CLOCK_DIV128 = 13U,
    RCC_AHB_CLOCK_DIV256 = 14U,
    RCC_AHB_CLOCK_DIV512 = 15U
} RCC_AHB_DIV;

// Clock Source
typedef enum
{
    SOURCE_HSI = 0U,
    SOURCE_HSE,
    SOURCE_PLL
} RCC_System_Clock_Source;

// System Clock Settings
typedef struct
{
    UInt32 SYSCLK;
    UInt32 HCLK;
    UInt32 APB1_CLOCK;
    UInt32 APB1_TIM_CLOCK;
    UInt32 APB2_CLOCK;
    UInt32 APB2_TIM_CLOCK;
} RCC_System_Clocks;

extern RCC_System_Clocks RCC_Clocks;

ESL_StatusTypeDef ESL_RCC_Init
(   
    RCC_PLLP_DIV PLLP_prescaler, 
    UInt16 PLLN_prescaler, 
    UInt16 PLLM_prescaler, 
    UInt16 PLLQ_prescaler, 
    RCC_APB_DIV APB1_prescaler, 
    RCC_APB_DIV APB2_prescaler, 
    RCC_AHB_DIV AHB_prescaler
);

#endif // __STM32F2xx_ESL_RCC_H