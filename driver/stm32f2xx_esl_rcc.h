/********************************************************************************************
 *  Filename: stm32f2xx_esl_rcc.h
 *  Author: Erik Fagerland
 *  Created On: 14/02/2024
 * 
 *  Brief:
 *  Header file of RCC
 * 
 *******************************************************************************************/
#ifndef __STM32F2xx_ESL_RCC_H
#define __STM32F2xx_ESL_RCC_H

#include "stm32f207xx.h"

/********************************************************************************************
*  USER CONFIGS
* 
*  Target sysclock of MCU. PLL, AHB and APB settings must be recalculated
*  in order to change clock! It's not enough to just change this definition! 
*/
#define RCC_SYSCLK_TARGET 120000000     // Target System Clock                  [MHz]
#define RCC_HSE_CRYSTAL_FREQ 8000000    // Frequency on the onboard HSE crystal [MHz]

/********************************************************************************************/

#define RCC_BASE 0x40023800U

#define FLASH_ACR_LATENCY_POS 0U

#define RCC_CR_PLLRDY               (1U << 25U)
#define RCC_CR_HSE_ON               (1U << 16U)
#define RCC_CR_HSI_ON               (1U << 0U)
#define RCC_CR_PLL_ON               (1U << 24U)        
#define RCC_CR_HSE_RDY              (1U << 17U)
#define RCC_CR_HSI_RDY              (1U << 1U)

// CPU Clock sources
#define RCC_CFGR_SW_HSI             (SOURCE_HSI << RCC_CFGR_SW_POS)
#define RCC_CFGR_SW_HSE             (SOURCE_HSE << RCC_CFGR_SW_POS)
#define RCC_CFGR_SW_PLL             (SOURCE_PLL << RCC_CFGR_SW_POS)

#define RCC_PLLCFGR_PLLP_POS        (16U)
#define RCC_PLLCFGR_PLLN_POS        (6U)
#define RCC_PLLCFGR_PLLM_POS        (0U)
#define RCC_PLLCFGR_PLLQ_POS        (24U)
#define RCC_PLLCFGR_PLL_SRC_POS     (22U)
#define RCC_PLLCFGR_PLL_SRC_HSE     (0x1UL << 22U)

#define RCC_CFGR_APB2_POS           (13U)
#define RCC_CFGR_APB1_POS           (10U)
#define RCC_CFGR_AHB_POS            (4U)
#define RCC_CFGR_SW_POS             (0U)  

// RCC AHB1ENR Register
#define RCC_AHB1ENR_GPIOAEN         (1U << 0U)
#define RCC_AHB1ENR_GPIOBEN         (1U << 1U)
#define RCC_AHB1ENR_GPIOCEN         (1U << 2U)
#define RCC_AHB1ENR_GPIODEN         (1U << 3U)
#define RCC_AHB1ENR_GPIOEEN         (1U << 4U)
#define RCC_AHB1ENR_GPIOFEN         (1U << 5U)
#define RCC_AHB1ENR_GPIOGEN         (1U << 6U)
#define RCC_AHB1ENR_GPIOHEN         (1U << 7U)
#define RCC_AHB1ENR_GPIOIEN         (1U << 8U)
#define RCC_AHB1ENR_CRCEN           (1U << 12U)
#define RCC_AHB1ENR_BKPSRAMEN       (1U << 18U)
#define RCC_AHB1ENR_DMA1EN          (1U << 21U)
#define RCC_AHB1ENR_DMA2EN          (1U << 22U)
#define RCC_AHB1ENR_ETHMAC          (1U << 25U)
#define RCC_AHB1ENR_ETHMACTXEN      (1U << 26U)
#define RCC_AHB1ENR_ETHMACRXEN      (1U << 27U)
#define RCC_AHB1ENR_ETHMACPTPEN     (1U << 28U)
#define RCC_AHB1ENR_OTGHSEN         (1U << 29U)
#define RCC_AHB1ENR_OTGHSULPIEN     (1U << 30U)

// RCC APB1ENR Register
#define RCC_APB1ENR_TIM2EN          (1U << 0U)
#define RCC_APB1ENR_TIM3EN          (1U << 1U)
#define RCC_APB1ENR_TIM4EN          (1U << 2U)
#define RCC_APB1ENR_TIM5EN          (1U << 3U)
#define RCC_APB1ENR_TIM6EN          (1U << 4U)
#define RCC_APB1ENR_TIM7EN          (1U << 5U)
#define RCC_APB1ENR_TIM12EN         (1U << 6U)
#define RCC_APB1ENR_TIM13EN         (1U << 7U)
#define RCC_APB1ENR_TIM14EN         (1U << 8U)
#define RCC_APB1ENR_WWDGEN          (1U << 11U)
#define RCC_APB1ENR_SPI2EN          (1U << 14U)
#define RCC_APB1ENR_SPI3EN          (1U << 15U)
#define RCC_APB1ENR_USART2EN        (1U << 17U)
#define RCC_APB1ENR_USART3EN        (1U << 18U)
#define RCC_APB1ENR_UART4EN         (1U << 19U)
#define RCC_APB1ENR_UART5EN         (1U << 20U)
#define RCC_APB1ENR_I2C1EN          (1U << 21U)
#define RCC_APB1ENR_I2C2EN          (1U << 22U)
#define RCC_APB1ENR_I2C3EN          (1U << 23U)
#define RCC_APB1ENR_CAN1EN          (1U << 25U)
#define RCC_APB1ENR_CAN2EN          (1U << 26U)
#define RCC_APB1ENR_PWREN           (1U << 28U)
#define RCC_APB1ENR_DACEN           (1U << 29U)

// RCC APB2ENR Register
#define RCC_APB2ENR_TIM1EN          (1U << 0U)
#define RCC_APB2ENR_TIM8EN          (1U << 1U)
#define RCC_APB2ENR_USART1EN        (1U << 4U)
#define RCC_APB2ENR_USART2EN        (1U << 5U)
#define RCC_APB2ENR_ADC1EN          (1U << 8U)
#define RCC_APB2ENR_ADC2EN          (1U << 9U)
#define RCC_APB2ENR_ADC3EN          (1U << 10U)
#define RCC_APB2ENR_SDIOEN          (1U << 11U)
#define RCC_APB2ENR_SPI1EN          (1U << 12U)
#define RCC_APB2ENR_SYSCFGEN        (1U << 14U)
#define RCC_APB2ENR_TIM9EN          (1U << 16U)
#define RCC_APB2ENR_TIM10EN         (1U << 17U)
#define RCC_APB2ENR_TIM11EN         (1U << 18U)

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