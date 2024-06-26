#include "gpio.h"
#include "stm32f2xx_esl_gpio.h"

void GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Set the two leds on the dev board as pushpull outputs
    GPIO_InitStruct.Pin = GPIO_PIN_14 | GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    ESL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // Set the user button on dev board as input with interrupt on the riding edge
    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    ESL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    // Set the PD5 and PD6 as UART
    GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    ESL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    // Set PB8 and PB9 to I2C
    GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    ESL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // Set initial state
    ESL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
    ESL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
}