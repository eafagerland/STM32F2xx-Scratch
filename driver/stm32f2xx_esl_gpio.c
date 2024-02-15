#include "stm32f2xx_esl_gpio.h"

void ESL_GPIO_Init(GPIO_Typedef* GPIOx, GPIO_InitTypeDef* GPIO_Init)
{
    UInt32 position = 0x00U;
    UInt32 iocurrent;
    UInt32 temp;

    while (((GPIO_Init->Pin) >> position) != 0x00u)
    {
        /* Get current io position */
        iocurrent = (GPIO_Init->Pin) & (1uL << position);

        if (iocurrent != 0x00u)
        {
            if((GPIO_Init->Mode & EXTI_MODE) != 0x00u)
            {
                // Set mode to input
                GPIOx->MODER |= (GPIO_Init->Mode << (position * 2U));

                // Clear rising edge setting and set new one
                temp = EXTI->RTSR;
                temp &= ~(iocurrent);
                if((GPIO_Init->Mode & TRIGGER_RISING) != 0x00u)
                {
                    temp |= iocurrent;
                }
                EXTI->RTSR = temp;

                // Clear falling edge setting and set new one
                temp = EXTI->FTSR;
                temp &= ~(iocurrent);
                if((GPIO_Init->Mode & TRIGGER_FALLING) != 0x00u)
                {
                    temp |= iocurrent;
                }
                EXTI->FTSR = temp;

                // Clear enable interrupt bit and set
                EXTI->IMR &= ~(1 << position);
                EXTI->IMR |= (1 << position);
                
                EXTI->EMR &= ~(1 << position);      // Clear event interrupt

                EXTI->PR |= (1 << position);        // Reset interrupt
                SYSCFG->EXTICR4 |= (2U << 4);       // Set PC[x] as source input for EXTI // TODO: Needs to select register dynamically
                ESL_NVIC_Enable(40);                // TODO: Needs to select position dynamically
            }

            if ((GPIO_Init->Mode & GPIO_MODE) == MODE_AF)
            {
                if (position < 8)
                {
                    GPIOx->AFR[0] &= ~(0b1111UL << (position * 4));
                    GPIOx->AFR[0] |= (GPIO_Init->Alternate << (position * 4));
                }
                else
                {
                    GPIOx->AFR[1] &= ~(0b1111UL << (position * 4));
                    GPIOx->AFR[1] |= (GPIO_Init->Alternate << (position * 4));
                }
                GPIOx->MODER |= (GPIO_Init->Mode << (position * 2U));
            }

            else
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