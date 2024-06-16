/********************************************************************************************
 *  @file   : os_kernel.h
 *  @author : Erik Fagerland
 *  @date   : 07/06/2024
 *
 *  @brief
 *  Implementation of RTOS Kernel with Thread shceduler
 *
 *******************************************************************************************/
#ifndef __OS_KERNEL_H__
#define __OS_KERNEL_H__

#include "stm32f207xx.h"

#define OS_MAX_TIMEOUT (0xFFFFFFFFUL)

typedef enum
{
    OS_OK = 0U,
    OS_TIMEOUT = 1
} OS_Status;

void os_kernel_init(void);
UInt8 os_kernel_add_threads(void(*task0)(void), void(*task1)(void), void(*task2)(void));
UInt8 os_kernel_new_thread(void(*task)(void), UInt16 stack_size);
void os_kernel_launch(void);
void os_thread_yield(void);
void os_task_delay(UInt32 delay_ms);
UInt32 os_get_tick(void);

#endif // __OS_KERNEL_H__