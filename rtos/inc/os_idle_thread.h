/********************************************************************************************
 *  @file   : os_idle_thread.h
 *  @author : Erik Fagerland
 *  @date   : 01/07/2024
 *
 *  @brief
 *  Implementation of RTOS Kernel Idle Thread
 *
 *******************************************************************************************/
#ifndef _OS_IDLE_THREAD_H_
#define _OS_IDLE_THREAD_H_
#include "os_kernel.h"

void os_idle_thread_init(void);
void os_idle_hook_callback(void);

extern tcb_type idle_thread_handler;

#endif // _OS_IDLE_THREAD_H_