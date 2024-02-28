/********************************************************************************************
 *  Filename: stm32f2xx_esl_i2c.c
 *  Author: Erik Fagerland
 *  Created On: 24/02/2024
 *
 *  Brief:
 *  Implementation of I2C
 *
 *******************************************************************************************/
#include "stm32f2xx_esl_i2c.h"

#define I2C_CR1_PERIPHERAL_ENABLE (1U << 0U)

#define I2C_FREQ_2MHZ       (0x2U)
#define I2C_FREQ_8MHZ       (0x8U)
#define I2C_SM_MODE         (0U)
#define I2C_FM_MODE         (1U)

#define I2C_CCR_FM_MODE_POS (15U)
#define I2C_CCR_CCR_POS     (0U)

#define I2C_CR1_START       (1U << 8U)
#define I2C_CR1_STOP        (1U << 9U)
#define I2C_CR1_ACK         (1U << 10U)
#define I2C_CR1_SWRST       (1U << 15U)

#define I2C_SR1_SB          (1U << 0U)
#define I2C_SR1_ADDR        (1U << 1U)
#define I2C_SR1_BTF         (1U << 2U)
#define I2C_SR1_TXE         (1U << 7U)
#define I2C_SR1_RXNE        (1U << 6U)

#define I2C_SR2_BUSY        (1U << 1U)

/********************************************************************************************
 *
 *******************************************************************************************/
ESL_StatusTypeDef ESL_I2C_Init(I2C_TypeDef *i2c)
{
    // Disable
    RESET_REG(i2c->CR1, I2C_CR1_PERIPHERAL_ENABLE);

    // Reset
    SET_REG(i2c->CR1, I2C_CR1_SWRST);
    RESET_REG(i2c->CR1, I2C_CR1_SWRST);

    // Set the Clock Frequency
    RESET_REG(i2c->CR2, (0x3FU << 0U));
    SET_REG(i2c->CR2, (0x1EU << 0U)); // Set to 2MHz

    // Set to SM mode
    RESET_REG(i2c->CCR, (1U << I2C_CCR_FM_MODE_POS));
    SET_REG(i2c->CCR, (I2C_SM_MODE << I2C_CCR_FM_MODE_POS));

    // Set the frequency SCL clock 100kHz
    RESET_REG(i2c->CCR, (0xFFFU << I2C_CCR_CCR_POS));
    SET_REG(i2c->CCR, (0x96U << I2C_CCR_CCR_POS));

    // set oar
    i2c->OAR1 = 0x4000UL;

    // Set rise and fall time
    RESET_REG(i2c->TRISE, (0x3FU << 0U));
    SET_REG(i2c->TRISE, (0x1F << 0U));

    // Enable (Must be done after setting CCR)
    SET_REG(i2c->CR1, I2C_CR1_PERIPHERAL_ENABLE);

    return ESL_OK;
}

/********************************************************************************************
 *
 *******************************************************************************************/
ESL_StatusTypeDef ESL_I2C_Master_Transmit(I2C_TypeDef *i2c, UInt8 address, UInt8 *buf, UInt32 len)
{
    // Wait while busy
    while (IS_BIT_SET(i2c->SR2, I2C_SR2_BUSY))
    {
    };

    // Set start condition
    SET_REG(i2c->CR1, I2C_CR1_START);

    // Wait for start bit generated
    while (!IS_BIT_SET(i2c->SR1, I2C_SR1_SB))
    {
    };

    // Send address with write bit
    i2c->DR = (address << 1U);

    // Wait for ACK
    while (!IS_BIT_SET(i2c->SR1, I2C_SR1_ADDR))
    {
    };

    // Reset ADDR
    volatile UInt32 sr1_reg = i2c->SR1;
    (void)sr1_reg;
    volatile UInt32 sr2_reg = i2c->SR2;
    (void)sr2_reg;

    UInt32 bytes_sent = 0;

    // Transmit data
    while (bytes_sent < len)
    {
        while ((!IS_BIT_SET(i2c->SR1, I2C_SR1_TXE)) && (!IS_BIT_SET(i2c->SR1, I2C_SR1_BTF)))
        {
        };
        i2c->DR = buf[bytes_sent];
        bytes_sent++;
    }

    // Wait for transfer complete
    while (!IS_BIT_SET(i2c->SR1, I2C_SR1_BTF))
    {
    };

    // Set stop condition
    SET_REG(i2c->CR1, I2C_CR1_STOP);

    return ESL_OK;
}

/********************************************************************************************
 *
 *******************************************************************************************/
ESL_StatusTypeDef ESL_I2C_Master_Receive(I2C_TypeDef *i2c, UInt8 address, UInt8 *buf, UInt32 len)
{
    // Wait while busy
    while (IS_BIT_SET(i2c->SR2, I2C_SR2_BUSY))
    {
    };

    // Enable ACK
    SET_REG(i2c->CR1, I2C_CR1_ACK);
    Bool is_ack_enabled = TRUE;

    // Set start condition
    SET_REG(i2c->CR1, I2C_CR1_START);

    // Wait for start bit generated
    while (!IS_BIT_SET(i2c->SR1, I2C_SR1_SB))
    {
    };

    // Send address with read bit
    address = (address << 1U);
    address |= (1U << 0U);
    i2c->DR = address;

    // Wait for ACK
    while (!IS_BIT_SET(i2c->SR1, I2C_SR1_ADDR))
    {
    };

    // Reset ACK now if only reading 1 byte
    if (is_ack_enabled && (len == 1))
    {
        RESET_REG(i2c->CR1, I2C_CR1_ACK);
        is_ack_enabled = FALSE;
    }

    // Reset ADDR
    volatile UInt32 sr1_reg = i2c->SR1;
    (void)sr1_reg;
    volatile UInt32 sr2_reg = i2c->SR2;
    (void)sr2_reg;

    if (len == 1)
    {
        // Set stop condition
        SET_REG(i2c->CR1, I2C_CR1_STOP);
    }

    UInt32 bytes_read = 0;

    // Read data
    while (bytes_read < len)
    {
        while (!IS_BIT_SET(i2c->SR1, I2C_SR1_RXNE))
        {
        };
        buf[bytes_read] = i2c->DR;
        bytes_read++;

        // Disable ACK to get nack after last byte received
        if ((len - bytes_read == 1) && is_ack_enabled)
        {
            RESET_REG(i2c->CR1, I2C_CR1_ACK);
            is_ack_enabled = FALSE;

            // Set stop condition
            SET_REG(i2c->CR1, I2C_CR1_STOP);
        }
    }

    RESET_REG(i2c->CR1, I2C_CR1_ACK);

    return ESL_OK;
}
