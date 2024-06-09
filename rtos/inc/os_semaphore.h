#ifndef __OS_SEMAPHORE_H__
#define __OS_SEMAPHORE_H__

#include "stm32f207xx.h"

typedef Int32 OS_Semaphore_Handle;

void os_semaphore_init(OS_Semaphore_Handle *semaphore, Int32 value);
void os_semaphore_set(OS_Semaphore_Handle *semaphore);
void os_semaphore_wait(OS_Semaphore_Handle *semaphore);

#endif // __OS_SEMAPHORE_H__