/********************************************************************************************
 *  Filename: os_memory.c
 *  Author: Erik Fagerland
 *  Created On: 11/06/2024
 *
 *  Brief:
 *  Implementation of heap memory manager
 *
 *******************************************************************************************/
#include "os_memory.h"

#define HEAP_SIZE               (0x8000UL) // 32kB
#define HEAP_TOP_ADDRESS        (0x2001BFF0UL - HEAP_SIZE)
#define MAX_ALLOCATION_ENTRIES  (64U)

static volatile UInt32 heap_index = 0;
static volatile UInt32 *new_adr_ptr = NULL;
static volatile UInt32 heap_bytes_used = 0;

typedef struct
{
    void *ptr;
    UInt16 size;
} AllocationEntry;

static AllocationEntry allocation_table[MAX_ALLOCATION_ENTRIES];

/********************************************************************************************
 *  Initializes the memory manager
 *******************************************************************************************/
void os_mem_init(void)
{
    heap_index = 0;
    heap_bytes_used = 0;
    new_adr_ptr = NULL;
    for (int i = 0; i < MAX_ALLOCATION_ENTRIES; ++i) 
    {
        allocation_table[i].ptr = NULL;
        allocation_table[i].size = 0;
    }
}

/********************************************************************************************
 *  Sets the heap index position and updates the current address pointer memory address
 *******************************************************************************************/
void set_heap_position(UInt16 position)
{
    heap_index = position;
    new_adr_ptr = (UInt32*)(HEAP_TOP_ADDRESS + heap_index);
}

/********************************************************************************************
 *  Allocates new memory in heap, returns pointer to the address. Returns NULL if failed
 *  to allocate memory.
 *******************************************************************************************/
void *allocate(UInt16 size) 
{
    // Ensure proper alignment of heap_pointer
    size = (size + 3) & (~3); // Round up to the nearest multiple of 4

    // Calculate the new address
    new_adr_ptr = (UInt32*)(HEAP_TOP_ADDRESS + heap_index);

    // Check if there's enough space left in the heap and reset back to top of heap
    if ((UInt32)new_adr_ptr + size > (UInt32)(HEAP_TOP_ADDRESS + HEAP_SIZE))
        set_heap_position(0);

    // Check if heap is full
    if (heap_bytes_used + size > HEAP_SIZE)
        return NULL;

    // Iterate through the allocated items to find free block in between
    for (int i = 0; i < MAX_ALLOCATION_ENTRIES; i++)
    {
        if (allocation_table[i].ptr == NULL)
            continue;

        // Check if current address is conflicting with allocation entries
        Int32 diff = (Int32)((UInt32)new_adr_ptr) - (UInt32)allocation_table[i].ptr;
        if (diff < allocation_table[i].size && diff >= 0)
        {
            set_heap_position(heap_index + allocation_table[i].size);
            continue;
        }
        else if (diff > -size && diff < 0)
        {
            set_heap_position(heap_index + allocation_table[i].size - diff);
            i = 0;
            continue;
        }
        break; // Found a free block
    }

    // Find a free entry in the allocation table
    int i;
    for (i = 0; i < MAX_ALLOCATION_ENTRIES; i++)
    {
        if (allocation_table[i].ptr == NULL)
        {
            allocation_table[i].ptr = (void *)new_adr_ptr;
            allocation_table[i].size = size;
            break;
        }
    }

    // Return NULL if no room in table
    if (i == MAX_ALLOCATION_ENTRIES)
        return NULL;

    heap_index += size;
    heap_bytes_used += size;

    return (void*)new_adr_ptr;
}

/********************************************************************************************
 *  Relocates existing pointer for new size to be allocated, returns pointer to address,
 *  returns NULL if failed
 *******************************************************************************************/
void *relocate(void *ptr, UInt16 size)
{
    // Get size of current ptr?
    UInt16 old_size = os_mem_size(ptr);

    // Store old ptr temp data
    UInt32 old_data[old_size];
    UInt32 *int_ptr = (UInt32 *)ptr;
    for (UInt32 i = 0; i < old_size; i++)
        old_data[i] = int_ptr[i];

    // Free current ptr
    free_mem(ptr);

    // Allocate new ptr with new size
    UInt32 *new_ptr = (UInt32*)allocate(size);

    if (new_ptr == NULL)
        return NULL;

    // Copy from temp to new ptr
    for (UInt32 i = 0; i < old_size; i++)
        new_ptr[i] = old_data[i];

    return (void*)new_ptr;
}

/********************************************************************************************
 *  Checks the allocation table to find and return the size allocated for that pointer.
 *******************************************************************************************/
UInt16 os_mem_size(void *ptr)
{
    for (UInt32 i = 0; i < MAX_ALLOCATION_ENTRIES; i++)
    {
        if (allocation_table[i].ptr == ptr)
            return allocation_table[i].size;
    }
    return 0;
}

/********************************************************************************************
 *  Frees the memory allocated at given pointer
 *******************************************************************************************/
void free_mem(void *ptr)
{
    // Search for the entry in the allocation table
    for (int i = 0; i < MAX_ALLOCATION_ENTRIES; ++i)
    {
        if (allocation_table[i].ptr == ptr)
        {
            // Found the allocated memory block, mark it as free
            heap_bytes_used -= allocation_table[i].size;
            allocation_table[i].ptr = NULL;
            allocation_table[i].size = 0;
            break;
        }
    }
}
