#include "stm32f2xx_esl_gpio.h"

void ESL_GPIO_Init(GPIO_Typedef* GPIOx, GPIO_InitTypeDef* GPIO_Init)
{
    UInt32 position = 0x00U;
    UInt32 iocurrent;

    while (((GPIO_Init->Pin) >> position) != 0x00u)
    {
        /* Get current io position */
        iocurrent = (GPIO_Init->Pin) & (1uL << position);

        if (iocurrent != 0x00u)
        {
            GPIOx->MODER |= (GPIO_Init->Mode << (position * 2U));
        }

        position++;
    }
}

void ESL_GPIO_WritePin(GPIO_Typedef* GPIOx, UInt16 GPIO_Pin, GPIO_PinState PinState)
{
    if (PinState == GPIO_PIN_SET) 
        GPIOx->ODR |= GPIO_Pin; // Set the pin
    else
        GPIOx->ODR &= ~GPIO_Pin; // Clear the pin
}

void ESL_GPIO_TogglePin(GPIO_Typedef* GPIOx, UInt16 GPIO_Pin)
{
    GPIO_PinState pinState = (GPIOx->ODR & GPIO_Pin) ? GPIO_PIN_SET : GPIO_PIN_RESET;
    ESL_GPIO_WritePin(GPIOx, GPIO_Pin, !pinState);
}

GPIO_PinState ESL_GPIO_Read_Pinstate(GPIO_Typedef* GPIOx, UInt16 GPIO_Pin)
{
    return (GPIOx->IDR & GPIO_Pin) ? GPIO_PIN_SET : GPIO_PIN_RESET;
}