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

/********************************************************************************************
 *  Initializes a semaphore
 *******************************************************************************************/
void os_semaphore_init(OS_Semaphore_Handle *semaphore, Int32 value)
{
    *semaphore = value;
}

/********************************************************************************************
 *  Sets the semaphore
 *******************************************************************************************/
void os_semaphore_give(OS_Semaphore_Handle *semaphore)
{
    __disable_irq();
    *semaphore += 1;
    __enable_irq(); 
}

/********************************************************************************************
 *  Wait for semaphore to be set
 *******************************************************************************************/
OS_Status os_semaphore_take(OS_Semaphore_Handle *semaphore, UInt32 timeout)
{
    __disable_irq();
    UInt32 tick_start = os_get_tick();
    while(*semaphore <= 0)
    {
        __disable_irq();
        UInt32 current_tick = os_get_tick();
        if (current_tick - tick_start >= timeout)
        {
            return OS_TIMEOUT;
            __enable_irq();
        }
        __enable_irq();
    }
    *semaphore -= 1;
    __enable_irq();
    return OS_OK;
}