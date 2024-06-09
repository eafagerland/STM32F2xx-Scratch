#include "stm32f2xx_esl.h"
#include "gpio.h"
#include "nvic.h"
#include "uart.h"
#include "eslstring.h"
#include "i2c.h"

int main(void)
{
    init_system_clocks();
    ESL_Init();
    GPIO_Init();
    NVIC_Init();
    UART2_Init();
    I2C_Init();

    // I2c test
    UInt8 tx_i2c_buf = 0xE3U;
    ESL_StatusTypeDef status = ESL_OK;
    status = ESL_I2C_Master_Transmit(I2C1, 0x40, &tx_i2c_buf, 1, 2000);
    if (status == ESL_TIMEOUT)
        print("I2C timeout!\r\n");
    else if (status == ESL_OK)
        print("I2C Write OK!\r\n");

    ESL_Delay(100);

    UInt8 rx_buf[3];
    status = ESL_I2C_Master_Receive(I2C1, 0x40, rx_buf, 3, 2000);
    if (status == ESL_TIMEOUT)
        print("I2C timeout!\r\n");
    else if (status == ESL_OK)
        print("I2C Read OK!\r\n");
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
    while (1)
    {
    }
}