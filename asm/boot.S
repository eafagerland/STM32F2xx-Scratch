# The Cortex M3 is a thumb only processor
.cpu cortex-m3
.thumb

// Vector Table
.word   0x20020000          //  - Stack top address
.word   _reset              //  - Reset
.word   none                //  - NMI
.word   HardFault_Handler   //  - HardFault
.word   none                //  - MemManage
.word   none                //  - BusFault
.word   none                //  - UsageFault
.word   none                //  - Reserved
.word   none                //  - Reserved
.word   none                //  - Reserved
.word   none                //  - Reserved
.word   none                //  - SVCall
.word   none                //  - DebugMonitor
.word   none                //  - Reserved
.word   none                //  - PendSV
.word   ESL_SysTick_Handler //  - SysTick
.word   none                //  0 WWDG
.word   none                //  1 PVD
.word   none                //  2 TAMP_STAMP
.word   none                //  3 RTC_WKUP
.word   none                //  4 FLASH
.word   none                //  5 RCC
.word   none                //  6 EXTI0
.word   none                //  7 EXTI1
.word   none		        //  8 EXTI2
.word   none                //  9 EXTI3
.word   none                //  10 EXTI4
.word   none                //  11 DMA1_Stream0
.word   none                //  12 DMA1_Stream1
.word   none                //  13 DMA1_Stream2
.word   none                //  14 DMA1_Stream3
.word   none                //  15 DMA1_Stream4
.word   none                //  16 DMA1_Stream5
.word   none                //  17 DMA1_Stream6
.word   none                //  18 ADC
.word   none                //  19 CAN1_TX
.word   none                //  20 CAN1_RX0
.word   none                //  21 CAN1_RX1
.word   none                //  22 CAN1_SCE
.word   none                //  23 EXTI9_5
.word   none                //  24 TIM1_BRK_TIM9
.word   TIM10_IRQHandler    //  25 TIM1_UP_TIM10
.word   none                //  26 TIM1_TRG_COM_TIM11
.word   none                //  27 TIM1_CC
.word   none                //  28 TIM2
.word   none                //  29 TIM3
.word   none                //  30 TIM4
.word   none                //  31 I2C1_EV
.word   none                //  32 I2C1_ER
.word   none                //  33 I2C2_EV
.word   none                //  34 I2C2_ER
.word   none                //  35 SPI1
.word   none                //  36 SPI2
.word   none                //  37 USART1
.word   none                //  38 USART2
.word   none                //  39 USART3
.word   none                //  40 EXTI15_10
.word   none                //  41 RTC_Alarm
.word   none                //  42 OTG_FS_WKUP
.word   none                //  43 TIM8_BRK_TIM12
.word   none                //  44 TIM8_UP_TIM13
.word   none                //  45 TIM8_TRG_COM_TIM14
.word   none                //  46 TIM8_CC
.word   none                //  47 DMA1_Stream7
.word   none                //  48 FSMC
.word   none                //  49 SDIO
.word   none                //  50 TIM5
.word   none                //  51 SPI3
.word   none                //  52 UART4
.word   none                //  53 UART5
.word   none                //  54 TIM6_DAC
.word   none                //  55 TIM7
.word   none                //  56 DMA2_Stream0
.word   none                //  57 DMA2_Stream1
.word   none                //  58 DMA2_Stream2
.word   none                //  59 DMA2_Stream3
.word   none                //  60 DMA2_Stream4
.word   none                //  61 ETH
.word   none                //  62 ETH_WKUP
.word   none                //  63 CAN2_TX
.word   none                //  64 CAN2_RX0
.word   none                //  65 CAN2_RX1
.word   none                //  66 CAN2_SCE
.word   none                //  67 OTG_FS
.word   none                //  68 DMA2_Stream5
.word   none                //  69 DMA2_Stream6
.word   none                //  70 DMA2_Stream7
.word   none                //  71 USART6
.word   none                //  72 I2C3_EV
.word   none                //  73 I2C3_ER
.word   none                //  74 OTG_HS_EP1_OUT
.word   none                //  75 OTG_HS_EP1_IN
.word   none                //  76 OTG_HS_WKUP
.word   none                //  77 OTG_HS
.word   none                //  78 DCMI
.word   none                //  79 CRYP
.word   none                //  80 HASH_RNG

.thumb_func
none:   b .

.thumb_func
_reset:
    bl main
    b .
