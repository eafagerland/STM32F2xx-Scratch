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
void os_semaphore_set(OS_Semaphore_Handle *semaphore)
{
    __disable_irq();
    *semaphore += 1;
    __enable_irq(); 
}

/********************************************************************************************
 *  Wait for semaphore to be set
 *******************************************************************************************/
void os_semaphore_wait(OS_Semaphore_Handle *semaphore)
{
    __disable_irq();
    while(*semaphore <= 0)
    {
        __disable_irq();
        __enable_irq();
    }
    *semaphore -= 1;
    __enable_irq();
}