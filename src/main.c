#include "stm32f2xx_esl.h"
#include "gpio.h"
#include "nvic.h"
#include "tim.h"
#include "uart.h"
#include "eslstring.h"
#include "rtc.h"
#include "i2c.h"
#include "sht21_core.h"

#define BLUE_LED GPIO_PIN_7
#define RED_LED GPIO_PIN_14
#define USER_BUTTON GPIO_PIN_13

#define UART_RX_BUFF_SIZE 10
char rx_buf[UART_RX_BUFF_SIZE] = {'\0'};

#define UART_TX_BUFF_SIZE 100
char tx_buf[UART_TX_BUFF_SIZE] = {'\0'};

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
    I2C_Init();

    // Init complete!
    print("Init Complete!\n\r");

    // Check if it started from a deepsleep standby
    if (ESL_PWR_Standby_Flagged())
    {
        print("Awww.. I was just sleeping!\n\r");
        ESL_RTC_Wakeup_IRQ_Disable();
    }

    // Delay test
    for (int i = 0; i < 10; i++)
    {
        ESL_GPIO_TogglePin(GPIOB, RED_LED);
        ESL_Delay(100);
    }

    // I2c test
    UInt8 tx_i2c_buf = 0xE3U;
    ESL_I2C_Master_Transmit(I2C1, 0x40, &tx_i2c_buf, 1);

    ESL_Delay(100);

    UInt8 rx_buf[3];
    ESL_I2C_Master_Receive(I2C1, 0x40, rx_buf, 3);

    volatile float temo = SHT21_Parse_Temp(rx_buf);
    (void)temo;

    // Print something fancy in terminal
    print("Enter something!\n\r");

    // UART IRQ Read test
    ESL_UARTx_Receive_IT(&uart2, (UInt8 *)rx_buf, UART_RX_BUFF_SIZE);

    // Test UART IT TX
    stringcopy(tx_buf, "Hello World! Sent on TX IRQ\n\r");
    ESL_UARTx_Transmit_IT(&uart2, (UInt8 *)tx_buf, stringlen(tx_buf));

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

    // Set wakeup to 30sec and enable irq
    ESL_RTC_Set_Wakeup(30);
    ESL_RTC_Wakeup_IRQ_Enable();

    ESL_Delay(5000);
    print("Going to sleep!\n\r");
    // Enable wakeup pin
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
static void init_system_clocks()
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
        else
        {
            if (ESL_UARTx_Transmit_IT(&uart2, (UInt8 *)tx_buf, stringlen(tx_buf)) == ESL_OK)
                ESL_GPIO_WritePin(GPIOB, RED_LED, GPIO_PIN_SET);
        }
        EXTI->PR |= GPIO_PIN_13; // Reset interrupt
    }
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

        ESL_GPIO_TogglePin(GPIOB, BLUE_LED);

        // Reset interrupt
        ESL_TIM_Reset_IRQ(TIM10);
    }
}

/********************************************************************************************
 *  Hardfault handler
 *******************************************************************************************/
void HardFault_Handler(void)
{
    ESL_GPIO_WritePin(GPIOB, RED_LED, GPIO_PIN_SET);
    ESL_GPIO_WritePin(GPIOB, BLUE_LED, GPIO_PIN_SET);
    while (1)
    {
    }
}

/********************************************************************************************
 *  Receive Callback for Uarts
 *******************************************************************************************/
void ESL_UARTx_Receive_Callback(UARTx_Handle_TypeDef *uart)
{
    if (uart->instance == UART2)
    {
        print("UART CB! Data: ");
        char buf[100] = {'\0'};
        stringcopy(buf, (const char *)uart->rx_buf);
        println(buf);
        memset(rx_buf, '\0', UART_RX_BUFF_SIZE);
        ESL_UARTx_Receive_IT(&uart2, (UInt8 *)rx_buf, UART_RX_BUFF_SIZE);
    }
}

/********************************************************************************************
 *  Transmit Callback for Uarts
 *******************************************************************************************/
void ESL_UARTx_Transmit_Callback(UARTx_Handle_TypeDef *uart)
{
    if (uart->instance == UART2)
    {
        print("TX DONE!\r\n");
        ESL_GPIO_WritePin(GPIOB, RED_LED, GPIO_PIN_RESET);
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
