#ifndef __STM32F207xx__H
#define __STM32F207xx__H

#define UInt32 		unsigned int
#define Int32 		signed int
#define UInt16 		unsigned short
#define Int16 		signed short
#define UInt8 		unsigned char
#define Int8 		signed char

#define EXTI_BASE 0x40013C00U
#define SYSCFG_BASE 0x40013800U

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
	UInt32 AFR[2];
} GPIO_Typedef;

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
} TIMx_Typedef;

typedef struct 
{
    volatile UInt32 ISER[3]; // 0x0 to 0xB - interrupt set-enable registers
    volatile UInt32 ICER[3]; // 0xC to 0x18 - interrupt clear-enable registers
    volatile UInt32 ISPR[3]; // 0x18 to 0x24 - interrupt set-pending registers
    volatile UInt32 ICPR[3]; // 0x24 to 0x30 - interrupt clear-pending registers
    volatile UInt32 IABR[3]; // 0x30 to 0x3C - interrupt active bit registers
} NVIC_Typedef;

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
} RCC_Typedef;

typedef struct
{
	UInt32 STK_CTRL;
	UInt32 STK_LOAD;
	UInt32 STK_VAL;
	UInt32 STK_CALIB;
} SysTick_Typedef;

typedef struct
{
	UInt32 IMR;
	UInt32 EMR;
	UInt32 RTSR;
	UInt32 FTSR;
	UInt32 SWIER;
	UInt32 PR;
} EXTI_Typedef;

typedef struct
{
	UInt32 MEMRM;
	UInt32 PMC;
	UInt32 EXTICR1;
	UInt32 EXTICR2;
	UInt32 EXTICR3;
	UInt32 EXTICR4;
	UInt32 CMPCR;
} SYSCFG_Typedef;

#define GPIOA 				((GPIO_Typedef *) GPIOA_BASE)
#define GPIOB				((GPIO_Typedef *) GPIOB_BASE)
#define GPIOC 				((GPIO_Typedef *) GPIOC_BASE)
#define GPIOD 				((GPIO_Typedef *) GPIOD_BASE)
#define GPIOE 				((GPIO_Typedef *) GPIOE_BASE)

#define TIM1				((TIMx_Typedef* ) TIM1_BASE)
#define TIM2				((TIMx_Typedef* ) TIM2_BASE)
#define TIM3				((TIMx_Typedef* ) TIM3_BASE)
#define TIM4				((TIMx_Typedef* ) TIM4_BASE)
#define TIM5				((TIMx_Typedef* ) TIM5_BASE)
#define TIM6				((TIMx_Typedef* ) TIM6_BASE)
#define TIM7				((TIMx_Typedef* ) TIM7_BASE)
#define TIM8				((TIMx_Typedef* ) TIM8_BASE)
#define TIM9				((TIMx_Typedef* ) TIM9_BASE)
#define TIM10				((TIMx_Typedef* ) TIM10_BASE)
#define TIM11				((TIMx_Typedef* ) TIM11_BASE)
#define TIM12				((TIMx_Typedef* ) TIM12_BASE)
#define TIM13				((TIMx_Typedef* ) TIM13_BASE)
#define TIM14				((TIMx_Typedef* ) TIM14_BASE)

#define NVIC				((NVIC_Typedef* ) NVIC_BASE)

#define RCC					((RCC_Typedef* ) RCC_BASE)

#define SYSTICK				((SysTick_Typedef* ) SYSTICK_BASE)

#define EXTI				((EXTI_Typedef* ) EXTI_BASE)

#define SYSCFG				((SYSCFG_Typedef* ) SYSCFG_BASE)

#endif // __STM32F207xx__H