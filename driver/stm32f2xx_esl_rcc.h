#ifndef __STM32F2xx_ESL_RCC_H
#define __STM32F2xx_ESL_RCC_H

#include "stm32f207xx.h"

#define RCC_BASE    0x40023800U

#define RCC_APBx_CLOCK_DIV0  0U
#define RCC_APBx_CLOCK_DIV2  100U
#define RCC_APBx_CLOCK_DIV4  101U
#define RCC_APBx_CLOCK_DIV8  110U
#define RCC_APBx_CLOCK_DIV16 111U

#define RCC_AHB_CLOCK_DIV0      0U
#define RCC_AHB_CLOCK_DIV2      1000U
#define RCC_AHB_CLOCK_DIV4      1001U
#define RCC_AHB_CLOCK_DIV8      1010U
#define RCC_AHB_CLOCK_DIV16     1011U
#define RCC_AHB_CLOCK_DIV64     1100U
#define RCC_AHB_CLOCK_DIV128    1101U
#define RCC_AHB_CLOCK_DIV256    1110U
#define RCC_AHB_CLOCK_DIV512    1111U

// Clock Source
typedef enum
{
    SOURCE_HSI = 0U,
    SOURCE_HSE,
    SOURCE_PLL
} RCC_System_Clock_Source;

void ESL_RCC_Init(UInt16 pllp, UInt16 plln, UInt16 pllm, UInt16 pllq, UInt16 apb1, UInt16 apb2, UInt16 ahb1);

#endif // __STM32F2xx_ESL_RCC_H