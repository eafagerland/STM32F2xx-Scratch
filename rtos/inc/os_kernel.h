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
#include "os_memory.h"

#define OS_MAX_TIMEOUT (0xFFFFFFFFUL)

typedef enum
{
    Thread_Priority_Low = 0U,
    Thread_Priority_Medium,
    Thread_Priority_High
} OS_Thread_Priority;

typedef enum
{
    OS_OK = 0U,
    OS_TIMEOUT = 1U,
    OS_ERROR = 2U,
} OS_Status;

struct tcb
{
    Int32 *stack_pt;        // 4bytes
    struct tcb *next_pt;    // 4bytes
    UInt32 priority;        // 4bytes
    UInt32 is_suspended;    // 4bytes
    Int32 tick_wakeup;      // 4bytes
}; // 20 bytes

typedef struct tcb tcb_type;

extern volatile UInt32 systick_count;
extern volatile UInt32 thread_count;
extern tcb_type *current_pt;
extern tcb_type *thread_array;

void os_idle_hook(void);
void os_kernel_init(void);
UInt8 os_kernel_new_thread(void(*task)(void), UInt16 stack_size, OS_Thread_Priority priority);
void os_kernel_launch(void);
void os_thread_yield(void);
void os_task_delay(UInt32 delay_ms);
UInt32 os_get_tick(void);

#endif // __OS_KERNEL_H__