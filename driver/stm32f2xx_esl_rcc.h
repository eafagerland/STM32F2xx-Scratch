#ifndef __STM32F2xx_ESL_RCC_H
#define __STM32F2xx_ESL_RCC_H

#include "stm32f207xx.h"

#define RCC_BASE    0x40023800U

#define RCC_APBx_CLOCK_DIV0  0U
#define RCC_APBx_CLOCK_DIV2  4U
#define RCC_APBx_CLOCK_DIV4  5U
#define RCC_APBx_CLOCK_DIV8  6U
#define RCC_APBx_CLOCK_DIV16 7U

#define RCC_AHB_CLOCK_DIV0      0U
#define RCC_AHB_CLOCK_DIV2      8U
#define RCC_AHB_CLOCK_DIV4      9U
#define RCC_AHB_CLOCK_DIV8      10U
#define RCC_AHB_CLOCK_DIV16     11U
#define RCC_AHB_CLOCK_DIV64     12U
#define RCC_AHB_CLOCK_DIV128    13U
#define RCC_AHB_CLOCK_DIV256    14U
#define RCC_AHB_CLOCK_DIV512    15U

// Clock Source
typedef enum
{
    SOURCE_HSI = 0U,
    SOURCE_HSE,
    SOURCE_PLL
} RCC_System_Clock_Source;

void ESL_RCC_Init(UInt16 pllp, UInt16 plln, UInt16 pllm, UInt16 pllq, UInt16 apb1, UInt16 apb2, UInt16 ahb1);

#endif // __STM32F2xx_ESL_RCC_H