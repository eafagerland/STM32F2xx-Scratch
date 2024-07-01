/********************************************************************************************
 *  @file   : os_idle_thread.c
 *  @author : Erik Fagerland
 *  @date   : 01/07/2024
 *
 *  @brief
 *  Implementation of RTOS Kernel Idle Thread
 *
 *******************************************************************************************/
#include "os_idle_thread.h"

#define IDLE_THREAD_STACK_SIZE      (64UL)

tcb_type idle_thread_handler;
static Int32 IDLE_STACK[IDLE_THREAD_STACK_SIZE];

/********************************************************************************************
 *  Idle thread loop
 *  Checks if the tick_wakeup is set which indicates it has just been called from
 *  scheduler, then calls idle hook.
 *******************************************************************************************/
static void os_idle_thread(void)
{
    while(1)
    {
        if (idle_thread_handler.tick_wakeup > 0)
        {
            idle_thread_handler.tick_wakeup = 0;
            os_idle_hook_callback();
        }
        __asm volatile("nop");
    }
}

/********************************************************************************************
 *  Initializes the Idle thread and its stack
 *******************************************************************************************/
void os_idle_thread_init(void)
{
    IDLE_STACK[IDLE_THREAD_STACK_SIZE - 1] = (1U << 24U);               // Set bit21 (T-bit) in PSR to 1, to operate in Thumb mode
    IDLE_STACK[IDLE_THREAD_STACK_SIZE - 2] = (Int32)(&os_idle_thread);  // Set PC register

    idle_thread_handler.priority = Thread_Priority_Low;
    idle_thread_handler.stack_pt = &IDLE_STACK[IDLE_THREAD_STACK_SIZE - 16];
    idle_thread_handler.next_pt = &idle_thread_handler;
    idle_thread_handler.tick_wakeup = 0;
}

/********************************************************************************************
 *  Weak defined idle hook callback. Called whenever Idle thread is activated from
 *  scheduler. To be implemented in user code.
 *******************************************************************************************/
__weak void os_idle_hook_callback(void)
{
    // To be implemented in user code! Callback whenever idle thread is called.
}
