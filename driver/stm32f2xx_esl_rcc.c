#include "stm32f2xx_esl_rcc.h"
#include "stm32f2xx_esl_systick.h"

#define RCC_CR_PLLRDY               (1 << 25)
#define RCC_CR_HSE_ON               (1 << 16)
#define RCC_CR_PLL_ON               (1 << 24)                       

// CPU Clock sources
#define RCC_CFGR_SW_HSI             (SOURCE_HSI << RCC_CFGR_SW_POS)
#define RCC_CFGR_SW_HSE             (SOURCE_HSE << RCC_CFGR_SW_POS)
#define RCC_CFGR_SW_PLL             (SOURCE_PLL << RCC_CFGR_SW_POS)

#define RCC_PLLCFGR_PLLP_POS        16
#define RCC_PLLCFGR_PLLN_POS        6
#define RCC_PLLCFGR_PLLM_POS        0
#define RCC_PLLCFGR_PLLQ_POS        24
#define RCC_PLLCFGR_PLL_SRC_POS     22
#define RCC_PLLCFGR_PLL_SRC_HSE     (0x1UL << 22)

#define RCC_CFGR_APB2_POS           13
#define RCC_CFGR_APB1_POS           10
#define RCC_CFGR_AHB_POS            4
#define RCC_CFGR_SW_POS             (0U)  

RCC_System_Clocks RCC_Clocks = {0};

static UInt16 Get_AHB_Div(RCC_AHB_DIV AHP)
{
    switch (AHP)
    {
        case RCC_AHB_CLOCK_DIV1: return 1;
        case RCC_AHB_CLOCK_DIV2: return 2;
        case RCC_AHB_CLOCK_DIV4: return 4;
        case RCC_AHB_CLOCK_DIV8: return 8;
        case RCC_AHB_CLOCK_DIV16: return 16;
        case RCC_AHB_CLOCK_DIV64: return 64;
        case RCC_AHB_CLOCK_DIV128: return 128;
        case RCC_AHB_CLOCK_DIV256: return 256;
        case RCC_AHB_CLOCK_DIV512: return 512;
    }
}

static UInt16 Get_APB_Div(RCC_APB_DIV APB)
{
    switch (APB)
    {
        case RCC_APBx_CLOCK_DIV1: return 1;
        case RCC_APBx_CLOCK_DIV2: return 2;
        case RCC_APBx_CLOCK_DIV4: return 4;
        case RCC_APBx_CLOCK_DIV8: return 8;
        case RCC_APBx_CLOCK_DIV16: return 16;
    }
}

static UInt16 Get_PLLP_Div(RCC_PLLP_DIV PLLP)
{
    switch (PLLP)
    {
        case RCC_PLLP_CLOCK_DIV2: return 2;
        case RCC_PLLP_CLOCK_DIV4: return 4;
        case RCC_PLLP_CLOCK_DIV6: return 6;
        case RCC_PLLP_CLOCK_DIV8: return 8;
    }
}

static void Calculate_RCC_Clocks(RCC_APB_DIV APB1_prescaler, RCC_APB_DIV APB2_prescaler, RCC_AHB_DIV AHB_prescaler)
{
    RCC_Clocks.SYSCLK           = RCC_SYSCLK_TARGET;
    RCC_Clocks.HCLK             = RCC_SYSCLK_TARGET;
    RCC_Clocks.APB1_CLOCK       = (RCC_SYSCLK_TARGET / Get_AHB_Div(AHB_prescaler)) / Get_APB_Div(APB1_prescaler);
    RCC_Clocks.APB1_TIM_CLOCK   = ((RCC_SYSCLK_TARGET / Get_AHB_Div(AHB_prescaler)) / Get_APB_Div(APB1_prescaler) * 2);
    RCC_Clocks.APB2_CLOCK       = (RCC_SYSCLK_TARGET / Get_AHB_Div(AHB_prescaler)) / Get_APB_Div(APB2_prescaler);
    RCC_Clocks.APB2_TIM_CLOCK   = ((RCC_SYSCLK_TARGET / Get_AHB_Div(AHB_prescaler)) / Get_APB_Div(APB2_prescaler) * 2);
}

static ESL_StatusTypeDef Is_SYSCLK_OK(UInt16 PLLM_prescaler, UInt16 PLLN_prescaler, RCC_PLLP_DIV PLLP_prescaler)
{
    UInt32 calculated_sysclk = ((RCC_HSE_CRYSTAL_FREQ / PLLM_prescaler) * PLLN_prescaler) / Get_PLLP_Div(PLLP_prescaler);
    if (calculated_sysclk != RCC_SYSCLK_TARGET)
        return ESL_ERROR;
    return ESL_OK;
}

ESL_StatusTypeDef ESL_RCC_Init
(   
    RCC_PLLP_DIV PLLP_prescaler, 
    UInt16 PLLN_prescaler, 
    UInt16 PLLM_prescaler, 
    UInt16 PLLQ_prescaler, 
    RCC_APB_DIV APB1_prescaler, 
    RCC_APB_DIV APB2_prescaler, 
    RCC_AHB_DIV AHB_prescaler
)
{
    RCC->CR |= RCC_CR_HSE_ON;               // Enable HSE
    RCC->CR &= ~(1 << 0);                   // Disable HSI

    while (!(RCC->CR & (1 << 17)));         // HSE Ready Flag
    //while (!(RCC->CR & (1 << 1)));         // HSI Ready Flag
    
    RCC->CR &= ~(1 << 24);                  // Disable PLL

    // CLear PLLQ and set new value
    RCC->PLLCFGR &= ~(0b1111UL << RCC_PLLCFGR_PLLQ_POS);
    RCC->PLLCFGR |= (PLLQ_prescaler << RCC_PLLCFGR_PLLQ_POS);          

    // Clear PLL source and set HSE as source
    RCC->PLLCFGR &= ~(1 << RCC_PLLCFGR_PLL_SRC_POS);
    RCC->PLLCFGR |= RCC_PLLCFGR_PLL_SRC_HSE; 

    // Clear PLLP and PLLN
    RCC->PLLCFGR &= ~(0b11UL << RCC_PLLCFGR_PLLP_POS);          
    RCC->PLLCFGR &= ~(0b111111111UL << RCC_PLLCFGR_PLLN_POS);

    // Set PLLP and PLLN
    RCC->PLLCFGR |= (PLLP_prescaler << RCC_PLLCFGR_PLLP_POS);          
    RCC->PLLCFGR |= (PLLN_prescaler << RCC_PLLCFGR_PLLN_POS); 

    // Clear PLLM and set new value
    RCC->PLLCFGR &= ~(0b111111UL);                              
    RCC->PLLCFGR |= (PLLM_prescaler << RCC_PLLCFGR_PLLM_POS);           

    RCC->CR |= RCC_CR_PLL_ON;           // PLL ON

    while (!(RCC->CR & RCC_CR_PLLRDY)); // Wait until PLL is ready

    // Clear APB2 Clock divider and set new value
    RCC->CFGR &= ~(0b111UL << RCC_CFGR_APB2_POS); 
    RCC->CFGR |= (APB2_prescaler << RCC_CFGR_APB2_POS);

    // Clear APB1 Clock divider and set new value
    RCC->CFGR &= ~(0b111UL << RCC_CFGR_APB1_POS);
    RCC->CFGR |= (APB1_prescaler << RCC_CFGR_APB1_POS);

    // Clear AHB Clock divider and set new value
    RCC->CFGR &= ~(0b1111UL << RCC_CFGR_AHB_POS);			
    RCC->CFGR |= (AHB_prescaler << RCC_CFGR_AHB_POS);

    RCC->CFGR &= ~(0x3UL);        // Clear SW bits
    RCC->CFGR |= RCC_CFGR_SW_PLL; // Set PLL as main clock source

    // Enable clock on GPIO Port A
    RCC->AHB1ENR |= (1 << 0);

    // Enable clock on GPIO Port B
    RCC->AHB1ENR |= (1 << 1);

    // Enable clock on GPIO Port C
    RCC->AHB1ENR |= (1 << 2);

    // Enable clock on GPIO Port D
    RCC->AHB1ENR |= (1 << 3);

    // Enable clock on TIM10
    RCC->APB2ENR |= (1 << 17);

    // Enable clock on TIM11
    RCC->APB2ENR |= (1 << 18);

    // Enable SYSCFG
    RCC->APB2ENR |= (1 << 14); // For EXTI on GPIO

    // Enable clock on USART2
    RCC->APB1ENR |= (1 << 17);

    // Calculate the current clock values
    Calculate_RCC_Clocks(APB1_prescaler, APB2_prescaler, AHB_prescaler);

    // Check if the current PLL settings matches the target SYSCLK, if not throw error
    if (Is_SYSCLK_OK(PLLM_prescaler, PLLN_prescaler, PLLP_prescaler) != ESL_OK)
        return ESL_ERROR;
    
    return ESL_OK;
}
