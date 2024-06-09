/********************************************************************************************
 *  Filename: stm32f2xx_esl_rcc.c
 *  Author: Erik Fagerland
 *  Created On: 14/02/2024
 *
 *  Brief:
 *  Implementation of the RCC config for MCU. Initializes the MCU clocks
 *  with some error checking.
 *
 *******************************************************************************************/
#include "stm32f2xx_esl_rcc.h"
#include "stm32f2xx_esl_systick.h"

RCC_System_Clocks RCC_Clocks = {0};

static void set_abpx_ahb_prescalers(RCC_APB_DIV APB1_prescaler,
                                    RCC_APB_DIV APB2_prescaler,
                                    RCC_AHB_DIV AHB_prescaler)
{
    // Clear APB2 Clock divider and set new value
    RESET_REG(RCC->CFGR, (0x7UL << RCC_CFGR_APB2_POS));
    SET_REG(RCC->CFGR, (APB2_prescaler << RCC_CFGR_APB2_POS));

    // Clear APB1 Clock divider and set new value
    RESET_REG(RCC->CFGR, (0x7UL << RCC_CFGR_APB1_POS));
    SET_REG(RCC->CFGR, (APB1_prescaler << RCC_CFGR_APB1_POS));

    // Clear AHB Clock divider and set new value
    RESET_REG(RCC->CFGR, (0xFUL << RCC_CFGR_AHB_POS));
    SET_REG(RCC->CFGR, (AHB_prescaler << RCC_CFGR_AHB_POS));
}

/********************************************************************************************
 *  Sets the PLL prescalers.
 *******************************************************************************************/
static void set_pll_prescalers(UInt16 PLLQ_prescaler,
                               UInt16 PLLN_prescaler,
                               UInt16 PLLM_prescaler,
                               RCC_PLLP_DIV PLLP_prescaler)
{
    // Disable PLL
    RESET_REG(RCC->CR, RCC_CR_PLL_ON);

    // CLear PLLQ and set new value
    RESET_REG(RCC->PLLCFGR, (0xFUL << RCC_PLLCFGR_PLLQ_POS));
    SET_REG(RCC->PLLCFGR, (PLLQ_prescaler << RCC_PLLCFGR_PLLQ_POS));

    // Clear PLL source and set HSE as source
    RESET_REG(RCC->PLLCFGR, (0x1UL << RCC_PLLCFGR_PLL_SRC_POS));
    SET_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLL_SRC_HSE);

    // Clear PLLP and PLLN
    RESET_REG(RCC->PLLCFGR, (0x3UL << RCC_PLLCFGR_PLLP_POS));
    RESET_REG(RCC->PLLCFGR, (0x1FFUL << RCC_PLLCFGR_PLLN_POS));

    // Set PLLP and PLLN
    SET_REG(RCC->PLLCFGR, (PLLP_prescaler << RCC_PLLCFGR_PLLP_POS));
    SET_REG(RCC->PLLCFGR, (PLLN_prescaler << RCC_PLLCFGR_PLLN_POS));

    // Clear PLLM and set new value
    RESET_REG(RCC->PLLCFGR, (0x3FUL));
    SET_REG(RCC->PLLCFGR, (PLLM_prescaler << RCC_PLLCFGR_PLLM_POS));

    // Switch PLL on
    SET_REG(RCC->CR, RCC_CR_PLL_ON);
}

/********************************************************************************************
 *  Transform the AHB enum into an integer correspending to its DIV num.
 *******************************************************************************************/
static UInt16 get_ahb_div(RCC_AHB_DIV AHP)
{
    switch (AHP)
    {
        case RCC_AHB_CLOCK_DIV1:    return 1U;
        case RCC_AHB_CLOCK_DIV2:    return 2U;
        case RCC_AHB_CLOCK_DIV4:    return 4U;
        case RCC_AHB_CLOCK_DIV8:    return 8U;
        case RCC_AHB_CLOCK_DIV16:   return 16U;
        case RCC_AHB_CLOCK_DIV64:   return 64U;
        case RCC_AHB_CLOCK_DIV128:  return 128U;
        case RCC_AHB_CLOCK_DIV256:  return 256U;
        case RCC_AHB_CLOCK_DIV512:  return 512U;
    }
    return 0U;
}

/********************************************************************************************
 *  Transform the APB enum into an integer correspending to its DIV num.
 *******************************************************************************************/
static UInt16 get_apb_div(RCC_APB_DIV APB)
{
    switch (APB)
    {
        case RCC_APBx_CLOCK_DIV1:   return 1U;
        case RCC_APBx_CLOCK_DIV2:   return 2U;
        case RCC_APBx_CLOCK_DIV4:   return 4U;
        case RCC_APBx_CLOCK_DIV8:   return 8U;
        case RCC_APBx_CLOCK_DIV16:  return 16U;
    }
    return 0U;
}

/********************************************************************************************
 *  Transform the PLLP enum into an integer correspending to its DIV num.
 *******************************************************************************************/
static UInt16 get_pllp_div(RCC_PLLP_DIV PLLP)
{
    switch (PLLP)
    {
        case RCC_PLLP_CLOCK_DIV2:   return 2U;
        case RCC_PLLP_CLOCK_DIV4:   return 4U;
        case RCC_PLLP_CLOCK_DIV6:   return 6U;
        case RCC_PLLP_CLOCK_DIV8:   return 8U;
    }
    return 0U;
}

/********************************************************************************************
 *  Calculates the different clock speeds into an exported variable so they can be accessed
 *  elsewhere for other use.
 *******************************************************************************************/
static void Calculate_RCC_Clocks(RCC_APB_DIV APB1_prescaler, RCC_APB_DIV APB2_prescaler, RCC_AHB_DIV AHB_prescaler)
{
    RCC_Clocks.SYSCLK = RCC_SYSCLK_TARGET;
    RCC_Clocks.HCLK = RCC_SYSCLK_TARGET;
    RCC_Clocks.APB1_CLOCK = (RCC_SYSCLK_TARGET / get_ahb_div(AHB_prescaler)) / get_apb_div(APB1_prescaler);
    RCC_Clocks.APB1_TIM_CLOCK = ((RCC_SYSCLK_TARGET / get_ahb_div(AHB_prescaler)) / get_apb_div(APB1_prescaler) * 2);
    RCC_Clocks.APB2_CLOCK = (RCC_SYSCLK_TARGET / get_ahb_div(AHB_prescaler)) / get_apb_div(APB2_prescaler);
    RCC_Clocks.APB2_TIM_CLOCK = ((RCC_SYSCLK_TARGET / get_ahb_div(AHB_prescaler)) / get_apb_div(APB2_prescaler) * 2);
}

/********************************************************************************************
 *  Function for testing if the calculated sysclk based on prescaler and crystal matches the
 *  target sysclk defined in header file.
 *******************************************************************************************/
static ESL_StatusTypeDef is_sysclk_ok(UInt16 PLLM_prescaler, UInt16 PLLN_prescaler, RCC_PLLP_DIV PLLP_prescaler)
{
    UInt32 calculated_sysclk = ((RCC_HSE_CRYSTAL_FREQ / PLLM_prescaler) * PLLN_prescaler) / get_pllp_div(PLLP_prescaler);
    if (calculated_sysclk != RCC_SYSCLK_TARGET)
        return ESL_ERROR;
    return ESL_OK;
}

/********************************************************************************************
 *  Initializes the MCU clocks based on given PLL, APB and AHB prescalers.
 *  Also sets the flash latency to 3WS. Returns error if target clock dont match
 *  with settings.
 *  Should be the first thing called in main function!
 *******************************************************************************************/
ESL_StatusTypeDef ESL_RCC_Init(
    RCC_PLLP_DIV PLLP_prescaler,
    UInt16 PLLN_prescaler,
    UInt16 PLLM_prescaler,
    UInt16 PLLQ_prescaler,
    RCC_APB_DIV APB1_prescaler,
    RCC_APB_DIV APB2_prescaler,
    RCC_AHB_DIV AHB_prescaler)
{
    /***************************************************************************
     *  Set flash interface latency to 3 wait states
     *  (Must be set on higher frequencies. ref table 3 flash programming manual)
     */
    SET_REG(FLASH_INTF->ACR, (FLASH_LATENCY_3WS << FLASH_ACR_LATENCY_POS));

    /**************************************************************************/

    SET_REG(RCC->CR, RCC_CR_HSE_ON);   // Enable HSE
    RESET_REG(RCC->CR, RCC_CR_HSI_ON); // Disable HSI

    // HSE Ready Flag
    while (!IS_BIT_SET(RCC->CR, RCC_CR_HSE_RDY))
        ;

    // Set PLL
    set_pll_prescalers(PLLQ_prescaler, PLLN_prescaler, PLLM_prescaler, PLLP_prescaler);

    // Wait until PLL is ready
    while (!IS_BIT_SET(RCC->CR, RCC_CR_PLLRDY))
        ;

    // Set APBx and AHB prescalers
    set_abpx_ahb_prescalers(APB1_prescaler, APB2_prescaler, AHB_prescaler);

    RESET_REG(RCC->CFGR, (0x3UL));       // Clear SW bits
    SET_REG(RCC->CFGR, RCC_CFGR_SW_PLL); // Set PLL as main clock source

    // Enable used perfs on AHB1
    SET_REG(RCC->AHB1ENR, (RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIODEN));

    // Enable used perfs on APB2
    SET_REG(RCC->APB2ENR, (RCC_APB2ENR_TIM10EN | RCC_APB2ENR_TIM11EN | RCC_APB2ENR_SYSCFGEN));

    // Enable used perfs on APB1
    SET_REG(RCC->APB1ENR, (RCC_APB1ENR_USART2EN | RCC_APB1ENR_PWREN | RCC_APB1ENR_I2C1EN));

    // Calculate the current clock values
    Calculate_RCC_Clocks(APB1_prescaler, APB2_prescaler, AHB_prescaler);

    // Check if the current PLL settings matches the target SYSCLK, if not throw error
    if (is_sysclk_ok(PLLM_prescaler, PLLN_prescaler, PLLP_prescaler) != ESL_OK)
        return ESL_ERROR;

    return ESL_OK;
}

void ESL_RCC_RTC_Enable(RCC_RTC_Clk_Src_TypeDef clock_source)
{
    if (clock_source == RTC_CLK_LSE)
    {
        // Turn on LSE (Low Speed External)
        RESET_REG(RCC->BDCR, RCC_BDCR_LSEON);
        SET_REG(RCC->BDCR, RCC_BDCR_LSEON);

        // Wait for ready flag
        while (!IS_BIT_SET(RCC->BDCR, RCC_BDCR_LSERDY))
        {
        }
    }

    // Set RTC Clock Source
    RESET_REG(RCC->BDCR, (0x3U << RCC_BDCR_RTCSEL_POS));
    SET_REG(RCC->BDCR, (clock_source << RCC_BDCR_RTCSEL_POS));

    // Enable RTC
    RESET_REG(RCC->BDCR, RCC_BDCR_RTCEN);
    SET_REG(RCC->BDCR, RCC_BDCR_RTCEN);
}
