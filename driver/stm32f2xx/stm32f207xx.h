/********************************************************************************************
 *  Filename: stm32f207xx.h
 *  Author: Erik Fagerland
 *  Created On: 13/02/2024
 *
 *  Brief:
 *  Contains all the typedefs for the registers on the STM32F207xx.
 *  Handlers to the registers are also declared here.
 *
 *******************************************************************************************/
#ifndef __STM32F207xx__H
#define __STM32F207xx__H

#define __weak __attribute__((weak))

#define UNUSED(x) ((void)x)

#define SET_REG(reg, x) ((reg) |= (x))
#define RESET_REG(reg, x) ((reg) &= ~(x))
#define IS_BIT_SET(reg, x) ((reg) & (x))

#define UInt32  unsigned int
#define Int32   signed int
#define UInt16  unsigned short
#define Int16   signed short
#define UInt8   unsigned char
#define Int8    signed char

typedef enum
{
    FALSE = 0U,
    TRUE
} Bool;

#define EXTI_BASE       (0x40013C00UL)
#define SYSCFG_BASE     (0x40013800UL)
#define FLASH_BASE      (0x40023C00UL)
#define DBGMCU_BASE     (0xE0042000UL)
#define SCB_BASE        (0xE000ED00UL)
#define PWR_BASE        (0x40007000UL)
#define RTC_BASE        (0x40002800UL)
#define I2C1_BASE       (0x40005400UL)
#define I2C2_BASE       (0x40005800UL)
#define I2C3_BASE       (0x40005C00UL)

typedef enum
{
    ESL_OK      = 0x00U,
    ESL_ERROR   = 0x01U,
    ESL_BUSY    = 0x02U,
    ESL_TIMEOUT = 0x03U
} ESL_StatusTypeDef;

typedef enum
{
    RTC_WKUP_IRQn   = 3U,
    TIM10_IRQn      = 25U,
    EXTI15_10_IRQn  = 40U,
    UART2_IRQn      = 38U
} ESL_IRQ_Pos_TypeDef;

typedef struct
{
    UInt32 MODER;
    UInt32 OTYPER;
    UInt32 OSPEEDR;
    UInt32 PUPDR;
    UInt32 IDR;
    UInt32 ODR;
    UInt32 BSRR;
    UInt32 LCKR;
    UInt32 AFR[2U];
} GPIO_TypeDef;

typedef struct
{
    UInt32 CR1;
    UInt32 CR2;
    UInt32 SMCR;
    UInt32 DIER;
    UInt32 SR;
    UInt32 EGR;
    UInt32 CCMR1;
    UInt32 CCMR2;
    UInt32 CCER;
    UInt32 CNT;
    UInt32 PSC;
    UInt32 ARR;
    UInt32 RESERVED1;
    UInt32 CCR1;
    UInt32 CCR2;
    UInt32 CCR3;
    UInt32 CCR4;
    UInt32 RESERVED2;
    UInt32 DCR;
    UInt32 DMAR;
    UInt32 TIM2_5_OR;
} TIMx_TypeDef;

typedef struct
{
    volatile UInt32 ISER[3U]; // 0x0 to 0xB - interrupt set-enable registers
    volatile UInt32 ICER[3U]; // 0xC to 0x18 - interrupt clear-enable registers
    volatile UInt32 ISPR[3U]; // 0x18 to 0x24 - interrupt set-pending registers
    volatile UInt32 ICPR[3U]; // 0x24 to 0x30 - interrupt clear-pending registers
    volatile UInt32 IABR[3U]; // 0x30 to 0x3C - interrupt active bit registers
} NVIC_TypeDef;

typedef struct
{
    UInt32 CR;
    UInt32 PLLCFGR;
    UInt32 CFGR;
    UInt32 CIR;
    UInt32 AHB1RSTR;
    UInt32 AHB2RSTR;
    UInt32 AHB3RSTR;
    UInt32 RESERVED;
    UInt32 APB1RSTR;
    UInt32 APB2RSTR;
    UInt32 RESERVED1;
    UInt32 RESERVED2;
    UInt32 AHB1ENR;
    UInt32 AHB2ENR;
    UInt32 AHB3ENR;
    UInt32 RESERVED3;
    UInt32 APB1ENR;
    UInt32 APB2ENR;
    UInt32 RESERVED4;
    UInt32 RESERVED5;
    UInt32 AHB1LPENR;
    UInt32 AHB2LPENR;
    UInt32 AHB3LPENR;
    UInt32 RESERVED6;
    UInt32 APB1LPENR;
    UInt32 APB2LPENR;
    UInt32 RESERVED7;
    UInt32 RESERVED8;
    UInt32 BDCR;
    UInt32 CSR;
    UInt32 RESERVED9;
    UInt32 RESERVED10;
    UInt32 SSCGR;
    UInt32 PLLI2SCFGR;
} RCC_TypeDef;

typedef struct
{
    UInt32 STK_CTRL;
    UInt32 STK_LOAD;
    UInt32 STK_VAL;
    UInt32 STK_CALIB;
} SysTick_TypeDef;

typedef struct
{
    UInt32 IMR;
    UInt32 EMR;
    UInt32 RTSR;
    UInt32 FTSR;
    UInt32 SWIER;
    UInt32 PR;
} EXTI_TypeDef;

typedef struct
{
    UInt32 MEMRM;
    UInt32 PMC;
    UInt32 EXTICR1;
    UInt32 EXTICR2;
    UInt32 EXTICR3;
    UInt32 EXTICR4;
    UInt32 CMPCR;
} SYSCFG_TypeDef;

typedef struct
{
    UInt32 SR;
    UInt32 DR;
    UInt32 BRR;
    UInt32 CR1;
    UInt32 CR2;
    UInt32 CR3;
    UInt32 GTPR;
} UARTx_TypeDef;

typedef struct
{
    UInt32 ACR;
    UInt32 KEYR;
    UInt32 OPTKEYR;
    UInt32 SR;
    UInt32 CR;
    UInt32 OPTCR;
} FLASH_Intf_TypeDef;

// Debug MCU Typedef
typedef struct
{
    UInt32 IDCODE;
    UInt32 CR;
    UInt32 APB1FZ;
    UInt32 APB2FZ;
} DBGMCU_TypeDef;

// System Control Block Typedef
typedef struct
{
    UInt32 CPUID;
    UInt32 ICSR;
    UInt32 VTOR;
    UInt32 AIRCR;
    UInt32 SCR;
    UInt32 CCR;
    UInt32 SHP[12U];
    UInt32 SHCSR;
    UInt32 CFSR;
    UInt32 HFSR;
    UInt32 DFSR;
    UInt32 MMFAR;
    UInt32 BFAR;
    UInt32 AFSR;
    UInt32 PFR[2U];
    UInt32 DFR;
    UInt32 ADR;
    UInt32 MMFR[4U];
    UInt32 ISAR[5U];
    UInt32 RESERVED0[5U];
    UInt32 CPACR;
} SCB_TypeDef;

// Power Register Typedef
typedef struct
{
    UInt32 CR;
    UInt32 CSR;
} PWR_TypeDef;

typedef struct
{
    UInt32 TR;
    UInt32 DR;
    UInt32 CR;
    UInt32 ISR;
    UInt32 PRER;
    UInt32 WUTR;
    UInt32 CALIBR;
    UInt32 ALRMAR;
    UInt32 ALRMBR;
    UInt32 WPR;
    UInt32 TSTR;
    UInt32 TSDR;
    UInt32 TAFCR;
    UInt32 BKPxR[20];
} RTC_TypeDef;

typedef struct
{
    UInt32 CR1;
    UInt32 CR2;
    UInt32 OAR1;
    UInt32 OAR2;
    UInt32 DR;
    UInt32 SR1;
    UInt32 SR2;
    UInt32 CCR;
    UInt32 TRISE;
} I2C_TypeDef;

#define GPIOA       ((GPIO_TypeDef *)GPIOA_BASE)
#define GPIOB       ((GPIO_TypeDef *)GPIOB_BASE)
#define GPIOC       ((GPIO_TypeDef *)GPIOC_BASE)
#define GPIOD       ((GPIO_TypeDef *)GPIOD_BASE)
#define GPIOE       ((GPIO_TypeDef *)GPIOE_BASE)

#define TIM1        ((TIMx_TypeDef *)TIM1_BASE)
#define TIM2        ((TIMx_TypeDef *)TIM2_BASE)
#define TIM3        ((TIMx_TypeDef *)TIM3_BASE)
#define TIM4        ((TIMx_TypeDef *)TIM4_BASE)
#define TIM5        ((TIMx_TypeDef *)TIM5_BASE)
#define TIM6        ((TIMx_TypeDef *)TIM6_BASE)
#define TIM7        ((TIMx_TypeDef *)TIM7_BASE)
#define TIM8        ((TIMx_TypeDef *)TIM8_BASE)
#define TIM9        ((TIMx_TypeDef *)TIM9_BASE)
#define TIM10       ((TIMx_TypeDef *)TIM10_BASE)
#define TIM11       ((TIMx_TypeDef *)TIM11_BASE)
#define TIM12       ((TIMx_TypeDef *)TIM12_BASE)
#define TIM13       ((TIMx_TypeDef *)TIM13_BASE)
#define TIM14       ((TIMx_TypeDef *)TIM14_BASE)

#define NVIC        ((NVIC_TypeDef *)NVIC_BASE)

#define RCC         ((RCC_TypeDef *)RCC_BASE)

#define SYSTICK     ((SysTick_TypeDef *)SYSTICK_BASE)

#define EXTI        ((EXTI_TypeDef *)EXTI_BASE)

#define SYSCFG      ((SYSCFG_TypeDef *)SYSCFG_BASE)

#define UART1       ((UARTx_TypeDef *)UART1_BASE)
#define UART2       ((UARTx_TypeDef *)UART2_BASE)
#define UART3       ((UARTx_TypeDef *)UART3_BASE)
#define UART4       ((UARTx_TypeDef *)UART4_BASE)
#define UART5       ((UARTx_TypeDef *)UART5_BASE)
#define UART6       ((UARTx_TypeDef *)UART6_BASE)

#define FLASH_INTF  ((FLASH_Intf_TypeDef *)FLASH_BASE)

#define DBGMCU      ((DBGMCU_TypeDef *)DBGMCU_BASE)

#define SCB         ((SCB_TypeDef *)SCB_BASE)
#define PWR         ((PWR_TypeDef *)PWR_BASE)

#define RTC         ((RTC_TypeDef *)RTC_BASE)

#define I2C1        ((I2C_TypeDef *)I2C1_BASE)

#endif // __STM32F207xx__H