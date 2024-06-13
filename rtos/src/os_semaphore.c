/********************************************************************************************
 *  @file   : os_semaphore.c
 *  @author : Erik Fagerland
 *  @date   : 09/06/2024
 *
 *  @brief
 *  Implementation of Semaphores to be used with RTOS
 *
 *******************************************************************************************/
#include "os_semaphore.h"

#define SEMAPHORE_TYPE_SIZE             (0x3U)
#define SEMAPHORE_TYPE_POS_START        ((sizeof(OS_Semaphore_Handle) * 8U) - SEMAPHORE_TYPE_SIZE)
#define SEMAPHORE_COUNTER_MAX           (0xFFFFL)

// Extracts the semaphore type bits
#define GET_SEMAPHORE_TYPE(handle)      (Semaphore_Type)(((*handle) >> SEMAPHORE_TYPE_POS_START) & SEMAPHORE_TYPE_SIZE)
// Maskes out the Semaphore type bits
#define GET_SEMAPHORE_VALUE(handle)     (Int32)((*semaphore) & SEMAPHORE_COUNTER_MAX)

typedef enum
{
    SEMAPHORE_NOT_INIT      = 0U,
    SEMAPHORE_BINARY_TYPE   = 1U,
    SEMAPHORE_COUNTING_TYPE = 2U
} Semaphore_Type;

/********************************************************************************************
 *  Creates a binary semaphore
 *******************************************************************************************/
void os_semaphore_create_binary(OS_Semaphore_Handle *semaphore)
{
    // Set Value
    *semaphore = 0;
    // Set Type
    *semaphore |= (SEMAPHORE_BINARY_TYPE << SEMAPHORE_TYPE_POS_START);
}

/********************************************************************************************
 *  Creates a counting semaphore
 *******************************************************************************************/
void os_semaphore_create_counting(OS_Semaphore_Handle *semaphore)
{
    // Set Value
    *semaphore = 0;
    // Set Type
    *semaphore |= (SEMAPHORE_COUNTING_TYPE << SEMAPHORE_TYPE_POS_START);
}

/********************************************************************************************
 *  Sets the semaphore
 *******************************************************************************************/
void os_semaphore_give(OS_Semaphore_Handle *semaphore)
{
    __disable_irq();
    Semaphore_Type semaphore_type = GET_SEMAPHORE_TYPE(semaphore);
    Int32 temp_semaphore_value = GET_SEMAPHORE_VALUE(semaphore);

    switch (semaphore_type)
    {
        case SEMAPHORE_BINARY_TYPE:
            temp_semaphore_value = 1;
        break;

        case SEMAPHORE_COUNTING_TYPE:
            if (temp_semaphore_value <= SEMAPHORE_COUNTER_MAX)
                temp_semaphore_value += 1;
        break;

        case SEMAPHORE_NOT_INIT:
            __enable_irq(); 
            return;
    }

    temp_semaphore_value |= (semaphore_type << SEMAPHORE_TYPE_POS_START);
    *semaphore = temp_semaphore_value;
    __enable_irq(); 
}

/********************************************************************************************
 *  Wait for semaphore to be set
 *******************************************************************************************/
OS_Status os_semaphore_take(OS_Semaphore_Handle *semaphore, UInt32 timeout)
{
    __disable_irq();

    Semaphore_Type semaphore_type = GET_SEMAPHORE_TYPE(semaphore);
    OS_Semaphore_Handle temp_semaphore_value = GET_SEMAPHORE_VALUE(semaphore);

    UInt32 tick_start = os_get_tick();
    while(temp_semaphore_value <= 0)
    {
        __disable_irq();

        temp_semaphore_value = GET_SEMAPHORE_VALUE(semaphore);

        UInt32 current_tick = os_get_tick();
        if (current_tick - tick_start >= timeout)
        {
            return OS_TIMEOUT;
            __enable_irq();
        }
        __enable_irq();
    }
    temp_semaphore_value -= 1;
    temp_semaphore_value |= (semaphore_type << SEMAPHORE_TYPE_POS_START);
    *semaphore = temp_semaphore_value;
    __enable_irq();
    return OS_OK;
}