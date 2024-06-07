#include "i2c.h"
#include "stm32f2xx_esl_i2c.h"

void I2C_Init(void)
{
    ESL_I2C_Init(I2C1);
}