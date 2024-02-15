#include "stm32f2xx_esl_rcc.h"

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
#define RCC_CFGR_AHB1_POS           4
#define RCC_CFGR_SW_POS             (0U)  

void ESL_RCC_Init(UInt16 pllp, UInt16 plln, UInt16 pllm, UInt16 pllq, UInt16 apb1, UInt16 apb2, UInt16 ahb1)
{
    RCC->CR |= RCC_CR_HSE_ON;               // Enable HSE
    RCC->CR &= ~(1 << 0);                   // Disable HSI

    while (!(RCC->CR & (1 << 17)));         // HSE Ready Flag
    //while (!(RCC->CR & (1 << 1)));         // HSI Ready Flag
    
    RCC->CR &= ~(1 << 24);                  // Disable PLL

    // CLear PLLQ and set new value
    RCC->PLLCFGR &= ~(0b1111UL << RCC_PLLCFGR_PLLQ_POS);
    RCC->PLLCFGR |= (pllq << RCC_PLLCFGR_PLLQ_POS);          

    // Clear PLL source and set HSE as source
    RCC->PLLCFGR &= ~(1 << RCC_PLLCFGR_PLL_SRC_POS);
    RCC->PLLCFGR |= RCC_PLLCFGR_PLL_SRC_HSE; 

    // Clear PLLP and PLLN
    RCC->PLLCFGR &= ~(0b11UL << RCC_PLLCFGR_PLLP_POS);          
    RCC->PLLCFGR &= ~(0b111111111UL << RCC_PLLCFGR_PLLN_POS);

    // Set PLLP and PLLN
    RCC->PLLCFGR |= (pllp << RCC_PLLCFGR_PLLP_POS);          
    RCC->PLLCFGR |= (plln << RCC_PLLCFGR_PLLN_POS); 

    // Clear PLLM and set new value
    RCC->PLLCFGR &= ~(0b111111UL);                              
    RCC->PLLCFGR |= (pllm << RCC_PLLCFGR_PLLM_POS);           

    RCC->CR |= RCC_CR_PLL_ON;           // PLL ON

    while (!(RCC->CR & RCC_CR_PLLRDY)); // Wait until PLL is ready

    // Clear APB2 Clock divider and set new value
    RCC->CFGR &= ~(0b111UL << RCC_CFGR_APB2_POS); 
    RCC->CFGR |= (apb2 << RCC_CFGR_APB2_POS);

    // Clear APB1 Clock divider and set new value
    RCC->CFGR &= ~(0b111UL << RCC_CFGR_APB1_POS);
    RCC->CFGR |= (apb1 << RCC_CFGR_APB1_POS);

    // Clear AHB Clock divider and set new value
    RCC->CFGR &= ~(0b1111UL << RCC_CFGR_AHB1_POS);			
    RCC->CFGR |= (ahb1 << RCC_CFGR_AHB1_POS);

    RCC->CFGR &= ~(0x3UL);        // Clear SW bits
    RCC->CFGR |= RCC_CFGR_SW_PLL; // Set PLL as main clock source

    // Enable clock on GPIO Port A
    RCC->AHB1ENR |= (1 << 0);

    // Enable clock on GPIO Port B
    RCC->AHB1ENR |= (1 << 1);

    // Enable clock on GPIO Port C
    RCC->AHB1ENR |= (1 << 2);

    // Enable clock on TIM10
    RCC->APB2ENR |= (1 << 17);

    // Enable clock on TIM11
    RCC->APB2ENR |= (1 << 18);

    // Enable SYSCFG
    RCC->APB2ENR |= (1 << 14);
}
