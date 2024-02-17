#include "stm32f2xx_esl.h"
#include "gpio.h"
#include "nvic.h"
#include "tim.h"
#include "uart.h"
#include "eslstring.h"

#define BLUE_LED GPIO_PIN_7
#define RED_LED GPIO_PIN_14
#define USER_BUTTON GPIO_PIN_13

/***********************************************
*	Function Prototypes 
************************************************/
void HardFault_Handler(void);
static void init_system_clocks();

int main(void)
{
	init_system_clocks();
	GPIO_Init();
	TIM_Init();
	NVIC_Init();
	UART2_Init();

	// Init complete!
	char buf[100];
	stringcopy(buf, "\nInit Complete!");
	println(buf);

	UInt16 timer_val_tim11 = TIM11->CNT;

	// Delay test
	for (int i = 0; i < 10; i++)
	{
		ESL_GPIO_WritePin(GPIOB, RED_LED, GPIO_PIN_SET);
		ESL_Delay(100);
		ESL_GPIO_WritePin(GPIOB, RED_LED, GPIO_PIN_RESET);
		ESL_Delay(100);
	}

	// Print num dest
	UInt32 num = 3200;
	char num_str[20];
	uint_to_string(num, num_str);
	println(num_str);

	// Neg num test
	Int32 num_neg = -3200;
	int_to_string(num_neg, num_str);
	println(num_str);

	// Print something fancy in terminal
	stringcopy(buf, "\nEnter Something!");
	const char* cat_test  = ", NOW!";
	stringcat(buf, cat_test);
	println(buf);

	//UART Read test
	char rx_buf[100];
	stringset(rx_buf, '\0', 100);
	ESL_UARTx_Read(&uart2, (UInt8*)rx_buf, 100, 20000);

	// UART Test, Respond with received data if available
	stringcopy(buf, "Received Data: ");
	print(buf);
	if (rx_buf[0] == '\0')
	{
		stringcopy(buf, "NO DATA");
		println(buf);
	}
	else
		println(rx_buf);

	ESL_Delay(1000);

	// Turn off red led before sleep
	ESL_GPIO_WritePin(GPIOB, RED_LED, GPIO_PIN_RESET);

	// Enter Stop mode deepsleep (wakeup with user button)
	ESL_PWR_Enter_Sleep(PWR_SLP_PPDS_STOP, PWR_SLP_LPDS_OFF); 

	while (1)
	{
		__wfi();
		// Button test
		/*if (ESL_GPIO_Read_Pinstate(GPIOC, USER_BUTTON) == GPIO_PIN_SET)
		{
			ESL_GPIO_TogglePin(GPIOB, RED_LED);
			ESL_Delay(500);
		}*/
		/*if (TIM11->CNT - timer_val_tim11 >= 10000) // Toggle led every 1sec
		{
			ESL_GPIO_TogglePin(GPIOB, RED_LED);
			timer_val_tim11 = TIM11->CNT;
		}*/
	}
}

static void init_system_clocks()
{
	// Init system clocks, go to hardfault handler if there was errors in settings (use STM32MXCube to get these values)
	if (ESL_RCC_Init(RCC_PLLP_CLOCK_DIV2, 240, 8, 4, RCC_APBx_CLOCK_DIV4, RCC_APBx_CLOCK_DIV2, RCC_AHB_CLOCK_DIV1) != ESL_OK)
		HardFault_Handler();
}

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
			print("Woke up from stop mode!\n\r");
		}
		else
		{
			print("Interrupt on user button was triggerd!\n\r");
			ESL_GPIO_TogglePin(GPIOB, RED_LED);
		}
		EXTI->PR |= GPIO_PIN_13;  // Reset interrupt
	}	
}

void ESL_TIM_IRQ_Handler(TIMx_Typedef* TIMx)
{
	if (TIMx == TIM10)
	{
		ESL_GPIO_TogglePin(GPIOB, BLUE_LED);

		// Reset interrupt
		ESL_TIM_Reset_IRQ(TIM10);
	}
}

void HardFault_Handler(void)
{
	ESL_GPIO_WritePin(GPIOB, RED_LED, GPIO_PIN_SET);
	ESL_GPIO_WritePin(GPIOB, BLUE_LED, GPIO_PIN_SET);
	while (1){}
}
