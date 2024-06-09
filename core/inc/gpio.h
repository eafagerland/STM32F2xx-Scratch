#ifndef __GPIO__H
#define __GPIO__H
#include "stm32f2xx_esl_gpio.h"

#define BLUE_LED    GPIO_PIN_7
#define RED_LED     GPIO_PIN_14
#define GREEN_LED   GPIO_PIN_0
#define LED_PORT    GPIOB

void GPIO_Init(void);

#endif // __GPIO__H