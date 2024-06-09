#include "stm32f2xx_esl.h"
#include "gpio.h"
#include "nvic.h"
#include "tim.h"
#include "uart.h"
#include "eslstring.h"
#include "rtc.h"

#define USER_BUTTON GPIO_PIN_13

/***********************************************
 *	Function Prototypes
 ************************************************/
void HardFault_Handler(void);
static void init_system_clocks();

int main(void)
{
    init_system_clocks();
    ESL_Init();
    GPIO_Init();
    NVIC_Init();
    UART2_Init();
    RTC_Init();

    // Set wakeup to 30sec and enable irq
    ESL_RTC_Set_Wakeup(30);
    ESL_RTC_Wakeup_IRQ_Enable();

    // Check if it started from a deepsleep standby
    if (ESL_PWR_Standby_Flagged())
    {
        print("Awww.. I was just sleeping!\n\r");
        ESL_RTC_Wakeup_IRQ_Disable();
    }

    print("Going to sleep!\n\r");
    // Enable wakeup pin (User Button)
    ESL_PWR_Enable_WKUP_Pin();
    // Enter Stop mode deepsleep (wakeup with user button)
    ESL_PWR_Enter_Sleep(PWR_SLP_PPDS_STB, PWR_SLP_LPDS_OFF);

    while (1)
    {
        __wfi();
    }
}

/********************************************************************************************
 *  Initializes the system clocks.
 *******************************************************************************************/
static void init_system_clocks(void)
{
    // Init system clocks, go to hardfault handler if there was errors in settings (use STM32MXCube to get these values)
    if (ESL_RCC_Init(RCC_PLLP_CLOCK_DIV2, 240, 8, 4, RCC_APBx_CLOCK_DIV4, RCC_APBx_CLOCK_DIV2, RCC_AHB_CLOCK_DIV1) != ESL_OK)
        HardFault_Handler();
}

/********************************************************************************************
 *  Callback for GPIO external interrupt line 15-10
 *******************************************************************************************/
void EXTI15_10_Handler(void)
{
    // Check if PIN13 was the source of the interrupt
    if (GPIO_EXTI_SOURCE(GPIO_PIN_13))
    {
        if (g_pwr_stop_mode_active)
        {
            g_pwr_stop_mode_active = FALSE;
            init_system_clocks(); // Need to re-init clocks since they were disabled at sleep
            ESL_SysTick_Resume(); // Resume the systick
            print("Woke up from sleep mode!\n\r");
        }
        EXTI->PR |= GPIO_PIN_13; // Reset interrupt
    }
}

/********************************************************************************************
 *  Hardfault handler
 *******************************************************************************************/
void HardFault_Handler(void)
{
    ESL_GPIO_WritePin(LED_PORT, RED_LED, GPIO_PIN_SET);
    ESL_GPIO_WritePin(LED_PORT, BLUE_LED, GPIO_PIN_SET);
    while (1)
    {
    }
}

/********************************************************************************************
 *  Callback for RTC wakeup interrupt
 *******************************************************************************************/
void ESL_Wakeup_IRQ_Handler(void)
{
    // Clear the RTC wakeup interrupt flag
    RTC->ISR &= ~RTC_ISR_WUTF;

    EXTI->PR |= (1U << 22U);
}