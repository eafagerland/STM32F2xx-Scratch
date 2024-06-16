/********************************************************************************************
 *  Filename: os_memory.h
 *  Author: Erik Fagerland
 *  Created On: 11/06/2024
 *
 *  Brief:
 *  Implementation of heap memory manager
 *
 *******************************************************************************************/
#ifndef __OS_MEMORY_H__
#define __OS_MEMORY_H__

#define NULL (void*)0

#include "stm32f207xx.h"

void os_mem_init(void);
void *allocate(UInt16 size);
void *relocate(void *ptr, UInt16 size);
UInt16 os_mem_size(void *ptr);
void free_mem(void *ptr);

#endif // __OS_MEMORY_H__