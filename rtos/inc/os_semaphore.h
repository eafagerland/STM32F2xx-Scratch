/********************************************************************************************
 *  @file   : os_semaphore.h
 *  @author : Erik Fagerland
 *  @date   : 09/06/2024
 *
 *  @brief
 *  Implementation of Semaphores to be used with RTOS
 *
 *******************************************************************************************/
#ifndef __OS_SEMAPHORE_H__
#define __OS_SEMAPHORE_H__

#include "stm32f207xx.h"
#include "os_kernel.h"

typedef Int32 OS_Semaphore_Handle;

void os_semaphore_init(OS_Semaphore_Handle *semaphore, Int32 value);
void os_semaphore_give(OS_Semaphore_Handle *semaphore);
OS_Status os_semaphore_take(OS_Semaphore_Handle *semaphore, UInt32 timeout);

#endif // __OS_SEMAPHORE_H__