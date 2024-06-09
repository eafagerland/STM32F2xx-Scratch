#ifndef __OS_KERNEL_H__
#define __OS_KERNEL_H__

#include "stm32f207xx.h"

#define PERIOD 100

void os_kernel_init(void);
UInt8 os_kernel_add_threads(void(*task0)(void), void(*task1)(void), void(*task2)(void));
void os_kernel_launch(UInt32 quanta);
void os_thread_yield(void);
void os_task_delay(UInt32 delay_ms);

#endif // __OS_KERNEL_H__