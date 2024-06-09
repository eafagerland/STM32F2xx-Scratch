#include "stm32f2xx_esl.h"
#include "gpio.h"
#include "tim.h"
#include "uart.h"
#include "eslstring.h"
#include "os_kernel.h"
#include "os_semaphore.h"

#define USER_BUTTON GPIO_PIN_13

#define QUANTA (1UL)

typedef UInt32 TaskProfiler;

TaskProfiler task0_profiler = 0;
TaskProfiler task1_profiler = 0;
TaskProfiler task2_profiler = 0;

OS_Semaphore_Handle semaphore;

void thread_0(void)
{
    os_semaphore_set(&semaphore);
    while(1)
    {
        task0_profiler++;
        os_semaphore_wait(&semaphore);
        print("Semaphore optained in thread 0!\r\n");
        ESL_GPIO_TogglePin(LED_PORT, GREEN_LED);
        os_semaphore_set(&semaphore);
        os_task_delay(1);
    }
}

void thread_1(void)
{
    while(1)
    {
        task1_profiler++;
        os_semaphore_wait(&semaphore);
        ESL_GPIO_TogglePin(LED_PORT, BLUE_LED);
        print("Giving semaphore in thread 1!\r\n");
        os_semaphore_set(&semaphore);
        os_task_delay(1);
    }
}

void thread_2(void)
{
    while(1)
    {
        task2_profiler++;
        ESL_GPIO_TogglePin(LED_PORT, RED_LED);
        os_task_delay(1000);
    }
}

/***********************************************
 *	Function Prototypes
 ************************************************/
void HardFault_Handler(void);
static void init_system_clocks();

int main(void)
{
    task0_profiler = 0;
    task1_profiler = 0;
    task2_profiler = 0;
    init_system_clocks();
    ESL_Init();
    GPIO_Init();
    TIM_Init();
    //NVIC_Init();
    UART2_Init();
    os_kernel_init();
    os_semaphore_init(&semaphore, 0);
    os_kernel_add_threads(&thread_0, &thread_1, &thread_2);
    os_kernel_launch(QUANTA);

    // Init complete!
    print("Init Complete!\n\r");
    ESL_GPIO_WritePin(LED_PORT, GREEN_LED, GPIO_PIN_SET);

    while (1)
    {
        __wfi();
    }

    return -1;
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
 *  Hardfault handler
 *******************************************************************************************/
void HardFault_Handler(void)
{
    ESL_GPIO_WritePin(LED_PORT, RED_LED, GPIO_PIN_SET);
    ESL_GPIO_WritePin(LED_PORT, BLUE_LED, GPIO_PIN_SET);
    ESL_GPIO_WritePin(LED_PORT, GREEN_LED, GPIO_PIN_SET);
    while (1)
    {
    }
}

/********************************************************************************************
 *  Callback for timers
 *******************************************************************************************/
void ESL_TIM_IRQ_Handler(TIMx_TypeDef *TIMx)
{
    if (TIMx == TIM10)
    {
        /*print("Thread1 CycleCount: ");
        char buf[10] = {0};
        uint_to_string(task0_profiler, buf);
        print(buf);

        print(", Thread2 CycleCount: ");
        char buf2[10] = {0};
        uint_to_string(task1_profiler, buf2);
        print(buf2);

        print(", Thread3 CycleCount: ");
        char buf3[10] = {0};
        uint_to_string(task2_profiler, buf3);
        print(buf3);

        print("\r\n");*/

        // Reset interrupt
        ESL_TIM_Reset_IRQ(TIM10);
    }
}
