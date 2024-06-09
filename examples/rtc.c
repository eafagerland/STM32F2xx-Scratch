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
    TIM_Init();
    NVIC_Init();
    UART2_Init();
    RTC_Init();

    // Set date and time if it has been reset
    if (!ESL_RTC_Is_Calender_Init())
    {
        // Set time
        ESL_RTC_Time_TypeDef time;
        time.hour = 1U;
        time.minute = 29U;
        time.second = 0U;
        time.format = 0U;
        ESL_RTC_Set_Time(time);

        // Set date
        ESL_RTC_Date_TypeDef date;
        date.month = 2U;
        date.date = 22U;
        date.year = 24U;
        date.weekday = THURSDAY;
        ESL_RTC_Set_Date(date);
    }
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
 *  Callback for timers
 *******************************************************************************************/
void ESL_TIM_IRQ_Handler(TIMx_TypeDef *TIMx)
{
    if (TIMx == TIM10)
    {
        // Print RTC Time Test
        char buf[180] = {'\0'};

        stringcopy(buf, "Time: ");
        ESL_RTC_Time_TypeDef time = ESL_RTC_Get_Time();

        // Hour
        char hour_string[20] = {'\0'};
        uint_to_string(time.hour, hour_string);
        stringcat(buf, hour_string);

        stringcat(buf, ":");

        // Minute
        char minute_string[20] = {'\0'};
        uint_to_string(time.minute, minute_string);
        stringcat(buf, minute_string);

        stringcat(buf, ":");

        // Seconds
        char second_string[20] = {'\0'};
        uint_to_string(time.second, second_string);
        stringcat(buf, second_string);

        // Date
        ESL_RTC_Date_TypeDef date = ESL_RTC_Get_Date();

        stringcat(buf, ", Date: ");
        char date_string[20] = {'\0'};
        uint_to_string(date.date, date_string);
        stringcat(buf, date_string);

        stringcat(buf, "/");

        // Month
        char month_string[20] = {'\0'};
        uint_to_string(date.month, month_string);
        stringcat(buf, month_string);

        stringcat(buf, "/");

        // Year
        char year_string[20] = {'\0'};
        uint_to_string(date.year, year_string);
        stringcat(buf, year_string);

        stringcat(buf, "  ");

        // Weekday
        switch (date.weekday)
        {
        case MONDAY:
            stringcat(buf, "Monday");
            break;
        case THUESDAY:
            stringcat(buf, "Thuesday");
            break;
        case WEDNESDAY:
            stringcat(buf, "Wednesday");
            break;
        case THURSDAY:
            stringcat(buf, "Thursday");
            break;
        case FRIDAY:
            stringcat(buf, "Friday");
            break;
        case SATURDAY:
            stringcat(buf, "Saturday");
            break;
        case SUNDAY:
            stringcat(buf, "Sunday");
            break;
        }

        println(buf);

        ESL_GPIO_TogglePin(LED_PORT, BLUE_LED);

        // Reset interrupt
        ESL_TIM_Reset_IRQ(TIM10);
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