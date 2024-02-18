/********************************************************************************************
 *  Filename: stm32f2xx_esl_gpio.c
 *  Author: Erik Fagerland
 *  Created On: 13/02/2024
 * 
 *  Brief:
 *  Implementation of the GPIO functions on the MCU
 * 
 *******************************************************************************************/
#include "stm32f2xx_esl_gpio.h"
#include "stm32f2xx_esl_nvic.h"

/********************************************************************************************
 *  Updates the GPIO registers with correct modes. 
 *******************************************************************************************/
void ESL_GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_Init)
{
    UInt32 position = 0x00U;
    UInt32 current_pin;
    UInt32 temp;

    // Iterate all the pins to see if struct patches current pin
    while (((GPIO_Init->Pin) >> position) != 0x00u)
    {
        // Get current pin position
        current_pin = IS_BIT_SET(GPIO_Init->Pin, (1U << position));

        if (current_pin != 0x00u)
        {
            // Set Pin as Interrupt (EXTI)
            if((GPIO_Init->Mode & EXTI_MODE) != 0x00u)
            {
                // Set mode to input
                SET_REG(GPIOx->MODER, (GPIO_Init->Mode << (position * 2U)));

                // Clear rising edge setting and set new one
                temp = EXTI->RTSR;
                RESET_REG(temp, current_pin);
                if((GPIO_Init->Mode & TRIGGER_RISING) != 0x00U)
                {
                    temp |= current_pin;
                }
                EXTI->RTSR = temp;

                // Clear falling edge setting and set new one
                temp = EXTI->FTSR;
                RESET_REG(temp, current_pin);
                if((GPIO_Init->Mode & TRIGGER_FALLING) != 0x00U)
                {
                    temp |= current_pin;
                }
                EXTI->FTSR = temp;

                // Clear enable interrupt bit and set
                RESET_REG(EXTI->IMR, (1U << position));
                SET_REG(EXTI->IMR, (1U << position));
                
                // Clear event interrupt
                RESET_REG(EXTI->EMR, (1U << position));

                // Reset interrupt
                SET_REG(EXTI->PR, (1U << position));

                // Set PC[x] as source input for EXTI   // TODO: Needs to select register dynamically
                SET_REG(SYSCFG->EXTICR4, (2U << 4U));
                ESL_NVIC_Enable_IRQ(EXTI15_10_IRQn);    // TODO: Needs to select position dynamically
            }

            // Set Pin as UART
            if ((GPIO_Init->Mode & GPIO_MODE) == MODE_AF)
            {
                if (position < 8U)
                {
                    RESET_REG(GPIOx->AFR[0], (0xFUL << (position * 4U)));
                    SET_REG(GPIOx->AFR[0], (GPIO_Init->Alternate << (position * 4U)));
                }
                else
                {
                    RESET_REG(GPIOx->AFR[1], (0xFUL << (position * 4U)));
                    SET_REG(GPIOx->AFR[1], (GPIO_Init->Alternate << (position * 4U)));
                }
                SET_REG(GPIOx->MODER, (GPIO_Init->Mode << (position * 2U)));
            }
            // Set Pin as output
            else
                SET_REG(GPIOx->MODER, (GPIO_Init->Mode << (position * 2U)));
        }
        position++;
    }
}

/********************************************************************************************
 *  Sets the output state of given pin/port with given state.
 *******************************************************************************************/
void ESL_GPIO_WritePin(GPIO_TypeDef* GPIOx, UInt16 GPIO_Pin, GPIO_PinState PinState)
{
    if (PinState == GPIO_PIN_SET) 
        GPIOx->ODR |= GPIO_Pin; // Set the pin
    else
        GPIOx->ODR &= ~GPIO_Pin; // Clear the pin
}

/********************************************************************************************
 *  Toggles the output state of the given pin/port.
 *******************************************************************************************/
void ESL_GPIO_TogglePin(GPIO_TypeDef* GPIOx, UInt16 GPIO_Pin)
{
    GPIO_PinState pinState = (GPIOx->ODR & GPIO_Pin) ? GPIO_PIN_SET : GPIO_PIN_RESET;
    ESL_GPIO_WritePin(GPIOx, GPIO_Pin, !pinState);
}

/********************************************************************************************
 *  Reads a input pin/port and returns its state.
 *******************************************************************************************/
GPIO_PinState ESL_GPIO_Read_Pinstate(GPIO_TypeDef* GPIOx, UInt16 GPIO_Pin)
{
    return (GPIOx->IDR & GPIO_Pin) ? GPIO_PIN_SET : GPIO_PIN_RESET;
}