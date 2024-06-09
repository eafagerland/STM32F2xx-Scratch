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
    volatile UInt32 ISER[8U];   // interrupt set-enable registers
    UInt32 RESERVED0[24U];
    volatile UInt32 ICER[8U];   // interrupt clear-enable registers
    UInt32 RESERVED1[24U];
    volatile UInt32 ISPR[8U];   // interrupt set-pending registers
    UInt32 RESERVED2[24U];
    volatile UInt32 ICPR[8U];   // interrupt clear-pending registers
    UInt32 RESERVED3[24U];
    volatile UInt32 IABR[8U];   // interrupt active bit registers
    UInt32 RESERVED4[56U];
    volatile UInt32 IP[240U];   // Interrupt Priority Register
    UInt32 RESERVED5[644U];
    volatile UInt32 STIR;       // Software Trigger Interrupt Register
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

/*
 *  Disables Interrupts byt setting I-bit in CPSR
*/
static inline void __disable_irq(void)
{
    __asm volatile ("cpsid i" : : : "memory");
}

/*
 *  Enables Interrupts byt clearing I-bit in CPSR
*/
static inline void __enable_irq(void)
{
    __asm volatile ("cpsie i" : : : "memory");
}

typedef enum
{
    /******  Cortex-M3 Processor Exceptions Numbers **************************************************************/
    NonMaskableInt_IRQn         = -14,    /*!< 2 Non Maskable Interrupt                                          */
    HardFault_IRQn              = -13,    /*!< 3 Hard Fault Interrupt                                            */
    MemoryManagement_IRQn       = -12,    /*!< 4 Cortex-M3 Memory Management Interrupt                           */
    BusFault_IRQn               = -11,    /*!< 5 Cortex-M3 Bus Fault Interrupt                                   */
    UsageFault_IRQn             = -10,    /*!< 6 Cortex-M3 Usage Fault Interrupt                                 */
    SVCall_IRQn                 = -5,     /*!< 11 Cortex-M3 SV Call Interrupt                                    */
    DebugMonitor_IRQn           = -4,     /*!< 12 Cortex-M3 Debug Monitor Interrupt                              */
    PendSV_IRQn                 = -2,     /*!< 14 Cortex-M3 Pend SV Interrupt                                    */
    SysTick_IRQn                = -1,     /*!< 15 Cortex-M3 System Tick Interrupt                                */
    /******  STM32 specific Interrupt Numbers ********************************************************************/
    WWDG_IRQn                   = 0,      /*!< Window WatchDog Interrupt                                         */
    PVD_IRQn                    = 1,      /*!< PVD through EXTI Line detection Interrupt                         */
    TAMP_STAMP_IRQn             = 2,      /*!< Tamper and TimeStamp interrupts through the EXTI line             */
    RTC_WKUP_IRQn               = 3,      /*!< RTC Wakeup interrupt through the EXTI line                        */
    FLASH_IRQn                  = 4,      /*!< FLASH global Interrupt                                            */
    RCC_IRQn                    = 5,      /*!< RCC global Interrupt                                              */
    EXTI0_IRQn                  = 6,      /*!< EXTI Line0 Interrupt                                              */
    EXTI1_IRQn                  = 7,      /*!< EXTI Line1 Interrupt                                              */
    EXTI2_IRQn                  = 8,      /*!< EXTI Line2 Interrupt                                              */
    EXTI3_IRQn                  = 9,      /*!< EXTI Line3 Interrupt                                              */
    EXTI4_IRQn                  = 10,     /*!< EXTI Line4 Interrupt                                              */
    DMA1_Stream0_IRQn           = 11,     /*!< DMA1 Stream 0 global Interrupt                                    */
    DMA1_Stream1_IRQn           = 12,     /*!< DMA1 Stream 1 global Interrupt                                    */
    DMA1_Stream2_IRQn           = 13,     /*!< DMA1 Stream 2 global Interrupt                                    */
    DMA1_Stream3_IRQn           = 14,     /*!< DMA1 Stream 3 global Interrupt                                    */
    DMA1_Stream4_IRQn           = 15,     /*!< DMA1 Stream 4 global Interrupt                                    */
    DMA1_Stream5_IRQn           = 16,     /*!< DMA1 Stream 5 global Interrupt                                    */
    DMA1_Stream6_IRQn           = 17,     /*!< DMA1 Stream 6 global Interrupt                                    */
    ADC_IRQn                    = 18,     /*!< ADC1, ADC2 and ADC3 global Interrupts                             */
    CAN1_TX_IRQn                = 19,     /*!< CAN1 TX Interrupt                                                 */
    CAN1_RX0_IRQn               = 20,     /*!< CAN1 RX0 Interrupt                                                */
    CAN1_RX1_IRQn               = 21,     /*!< CAN1 RX1 Interrupt                                                */
    CAN1_SCE_IRQn               = 22,     /*!< CAN1 SCE Interrupt                                                */
    EXTI9_5_IRQn                = 23,     /*!< External Line[9:5] Interrupts                                     */
    TIM1_BRK_TIM9_IRQn          = 24,     /*!< TIM1 Break interrupt and TIM9 global interrupt                    */
    TIM1_UP_TIM10_IRQn          = 25,     /*!< TIM1 Update Interrupt and TIM10 global interrupt                  */
    TIM1_TRG_COM_TIM11_IRQn     = 26,     /*!< TIM1 Trigger and Commutation Interrupt and TIM11 global interrupt */
    TIM1_CC_IRQn                = 27,     /*!< TIM1 Capture Compare Interrupt                                    */
    TIM2_IRQn                   = 28,     /*!< TIM2 global Interrupt                                             */
    TIM3_IRQn                   = 29,     /*!< TIM3 global Interrupt                                             */
    TIM4_IRQn                   = 30,     /*!< TIM4 global Interrupt                                             */
    I2C1_EV_IRQn                = 31,     /*!< I2C1 Event Interrupt                                              */
    I2C1_ER_IRQn                = 32,     /*!< I2C1 Error Interrupt                                              */
    I2C2_EV_IRQn                = 33,     /*!< I2C2 Event Interrupt                                              */
    I2C2_ER_IRQn                = 34,     /*!< I2C2 Error Interrupt                                              */  
    SPI1_IRQn                   = 35,     /*!< SPI1 global Interrupt                                             */
    SPI2_IRQn                   = 36,     /*!< SPI2 global Interrupt                                             */
    USART1_IRQn                 = 37,     /*!< USART1 global Interrupt                                           */
    USART2_IRQn                 = 38,     /*!< USART2 global Interrupt                                           */
    USART3_IRQn                 = 39,     /*!< USART3 global Interrupt                                           */
    EXTI15_10_IRQn              = 40,     /*!< External Line[15:10] Interrupts                                   */
    RTC_Alarm_IRQn              = 41,     /*!< RTC Alarm (A and B) through EXTI Line Interrupt                   */
    OTG_FS_WKUP_IRQn            = 42,     /*!< USB OTG FS Wakeup through EXTI line interrupt                     */    
    TIM8_BRK_TIM12_IRQn         = 43,     /*!< TIM8 Break Interrupt and TIM12 global interrupt                   */
    TIM8_UP_TIM13_IRQn          = 44,     /*!< TIM8 Update Interrupt and TIM13 global interrupt                  */
    TIM8_TRG_COM_TIM14_IRQn     = 45,     /*!< TIM8 Trigger and Commutation Interrupt and TIM14 global interrupt */
    TIM8_CC_IRQn                = 46,     /*!< TIM8 Capture Compare Interrupt                                    */
    DMA1_Stream7_IRQn           = 47,     /*!< DMA1 Stream7 Interrupt                                            */
    FSMC_IRQn                   = 48,     /*!< FSMC global Interrupt                                             */
    SDIO_IRQn                   = 49,     /*!< SDIO global Interrupt                                             */
    TIM5_IRQn                   = 50,     /*!< TIM5 global Interrupt                                             */
    SPI3_IRQn                   = 51,     /*!< SPI3 global Interrupt                                             */
    UART4_IRQn                  = 52,     /*!< UART4 global Interrupt                                            */
    UART5_IRQn                  = 53,     /*!< UART5 global Interrupt                                            */
    TIM6_DAC_IRQn               = 54,     /*!< TIM6 global and DAC1&2 underrun error  interrupts                 */
    TIM7_IRQn                   = 55,     /*!< TIM7 global interrupt                                             */
    DMA2_Stream0_IRQn           = 56,     /*!< DMA2 Stream 0 global Interrupt                                    */
    DMA2_Stream1_IRQn           = 57,     /*!< DMA2 Stream 1 global Interrupt                                    */
    DMA2_Stream2_IRQn           = 58,     /*!< DMA2 Stream 2 global Interrupt                                    */
    DMA2_Stream3_IRQn           = 59,     /*!< DMA2 Stream 3 global Interrupt                                    */
    DMA2_Stream4_IRQn           = 60,     /*!< DMA2 Stream 4 global Interrupt                                    */
    ETH_IRQn                    = 61,     /*!< Ethernet global Interrupt                                         */
    ETH_WKUP_IRQn               = 62,     /*!< Ethernet Wakeup through EXTI line Interrupt                       */
    CAN2_TX_IRQn                = 63,     /*!< CAN2 TX Interrupt                                                 */
    CAN2_RX0_IRQn               = 64,     /*!< CAN2 RX0 Interrupt                                                */
    CAN2_RX1_IRQn               = 65,     /*!< CAN2 RX1 Interrupt                                                */
    CAN2_SCE_IRQn               = 66,     /*!< CAN2 SCE Interrupt                                                */
    OTG_FS_IRQn                 = 67,     /*!< USB OTG FS global Interrupt                                       */
    DMA2_Stream5_IRQn           = 68,     /*!< DMA2 Stream 5 global interrupt                                    */
    DMA2_Stream6_IRQn           = 69,     /*!< DMA2 Stream 6 global interrupt                                    */
    DMA2_Stream7_IRQn           = 70,     /*!< DMA2 Stream 7 global interrupt                                    */
    USART6_IRQn                 = 71,     /*!< USART6 global interrupt                                           */
    I2C3_EV_IRQn                = 72,     /*!< I2C3 event interrupt                                              */
    I2C3_ER_IRQn                = 73,     /*!< I2C3 error interrupt                                              */
    OTG_HS_EP1_OUT_IRQn         = 74,     /*!< USB OTG HS End Point 1 Out global interrupt                       */
    OTG_HS_EP1_IN_IRQn          = 75,     /*!< USB OTG HS End Point 1 In global interrupt                        */
    OTG_HS_WKUP_IRQn            = 76,     /*!< USB OTG HS Wakeup through EXTI interrupt                          */
    OTG_HS_IRQn                 = 77,     /*!< USB OTG HS global interrupt                                       */
    DCMI_IRQn                   = 78,     /*!< DCMI global interrupt                                             */
    RNG_IRQn                    = 80      /*!< Rng global interrupt                                              */
} IRQn_Type;

#endif // __STM32F207xx__H