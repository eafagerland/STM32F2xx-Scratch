#include "gpio.h"
#include "stm32f2xx_esl_gpio.h"

void GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_PIN_14 | GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	ESL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_13;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	ESL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}