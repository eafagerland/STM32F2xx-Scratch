/********************************************************************************************
 *  @file   : os_kernel.c
 *  @author : Erik Fagerland
 *  @date   : 07/06/2024
 *
 *  @brief
 *  Implementation of RTOS Kernel with Thread shceduler
 *
 *******************************************************************************************/
#include "os_kernel.h"
#include "os_memory.h"
#include "stm32f2xx_esl_systick.h"
#include "stm32f2xx_esl_nvic.h"
#include "stm32f2xx_esl_rcc.h"

#define RTOS_TICKRATE_HZ    (1000UL)

#define INTCTRL             (*((volatile UInt32 *)0xE000ED04))
#define PENDSTSET           (1U << 26U)

static UInt32 systick_count;
static UInt32 MILLIS_PRESCALER;

struct tcb
{
    Int32 *stack_pt;
    struct tcb *next_pt;
};

typedef struct tcb tcb_type;

static tcb_type *thread_array = NULL;
static tcb_type *current_pt = NULL;

static UInt16 thread_count = 0;

Int32 **TCB_STACK = NULL;

// Function Prototypes
void os_scheduler_launch(void);
void os_scheduler_round_robin(void);

/********************************************************************************************
 *  Creates a new thread in HEAP memory
 *******************************************************************************************/
UInt8 os_kernel_new_thread(void(*task)(void), UInt16 stack_size)
{
    __disable_irq();

    if (thread_array == NULL)
        thread_array = (tcb_type *)allocate((thread_count + 1) * sizeof(tcb_type));
    else
        thread_array = (tcb_type *)relocate(thread_array, (thread_count + 1) * sizeof(tcb_type));

    if (thread_array == NULL)
    {
        __enable_irq();
        return 1;
    }

    // Allocate memory for number of threads pointers to Int32
    if (TCB_STACK == NULL) 
        TCB_STACK = (Int32 **)allocate((thread_count + 1) * sizeof(Int32 *));
    else
        TCB_STACK = (Int32 **)relocate(TCB_STACK, (thread_count + 1) * sizeof(Int32 *));

    if (TCB_STACK == NULL)
    {
        __enable_irq();
        return 1;
    }

    // Allocate memory for thread stack
    TCB_STACK[thread_count] = (Int32 *)allocate(stack_size * sizeof(Int32));
    if (TCB_STACK[thread_count] == NULL)
    {
        __enable_irq();
        return 1;
    }

    tcb_type *thread = &thread_array[thread_count];

    thread->stack_pt = &TCB_STACK[thread_count][stack_size - 16];

    current_pt = &thread_array[0]; // Set the current thread pointer to first thread

    // Iterate all the threads to set the schedule
    for (UInt32 i = 0; i < thread_count + 1; i++)
    {   
        if (i == thread_count)
            thread_array[i].next_pt = &thread_array[0]; // If last thread, set next_pt to first thread

        if (i != 0)
            thread_array[i - 1].next_pt = &thread_array[i]; // Set the previous thread's next_pt to this
    }

    TCB_STACK[thread_count][stack_size - 1] = (1U << 24U);      // Set bit21 (T-bit) in PSR to 1, to operate in Thumb mode
    TCB_STACK[thread_count][stack_size - 2] = (Int32)(task);    // Initialize program counter (PC)

    thread_count++;

    __enable_irq();

    return 0;
}

/********************************************************************************************
 *  Initializes the kernel
 *******************************************************************************************/
void os_kernel_init(void)
{
    thread_array = NULL;
    TCB_STACK = NULL;
    MILLIS_PRESCALER = (RCC_Clocks.SYSCLK / RTOS_TICKRATE_HZ);
    systick_count = 0;
    thread_count = 0;
    current_pt = NULL;

    os_mem_init();
}

/********************************************************************************************
 *  Launches the kernel
 *******************************************************************************************/
void os_kernel_launch(void)
{
	__disable_irq();
    // Reset systick
    SYSTICK->STK_CTRL = 0;

    // Clear systick current value register
    SYSTICK->STK_VAL = 0;

    // Load quanta
    SYSTICK->STK_LOAD = MILLIS_PRESCALER - 1;

    // Set systick to low priority
    ESL_NVIC_Set_Priority(SysTick_IRQn, 15);

    // Enable Systick, Tickint and set AHB clock
    SET_REG(SYSTICK->STK_CTRL, (SYSTICK_ENABLE | SYSTICK_TICKINT_ENABLE | SYSTICK_CLK_SRC_AHB));

    // Launch Scheduler
    os_scheduler_launch();
}

/********************************************************************************************
 *  Systick interrupt handler. Increments the systick counter.
 *  If the quenta has elapsed it will switch to next thread
 *******************************************************************************************/
// When exception occurs these registers are automatically saved onto the stack: r0,r1,r2,r3,r12,lr,pc,psr
__attribute__((naked)) void ESL_SysTick_Handler(void)
{
    __asm volatile
    (
        "CPSID      I                   \n" // Disable interrupts

        // Increment SysTick Counter
        "LDR        R0,=systick_count   \n" // Load address of systick count into R0
        "LDR        R0,[R0]             \n" // Load value of systick_count into R0
        "ADD        R0,R0,#1            \n" // Increment systick_count

        "LDR        R1,=systick_count   \n" // Load address of systick_count into R3
        "STR        R0,[R1]             \n" // Store incremented value back to systick_count

        // SUSPEND CURRENT THREAD
        "PUSH       {R4-R11}            \n" // Save r4,r5,r6,r7,r8,r9,r10,11

        "LDR        R0,=current_pt      \n" // Load address of current_pt into r0
        "LDR        R1,[R0]             \n" // Load r1 from address equals r0, i.e. r1 = current_pt
        "STR        SP,[R1]             \n" // Store Cortex-M SP at address equals r1, i.e Save SP into tcb

        // CHOOSE NEXT THREAD      
        "LDR        R1,[R1,#4]          \n" // Load r1 from a location 4byte above address r1, i.e r1 = current_pt->next
        "STR        R1,[R0]             \n" // Store r1 at address equal r0, i.e current_pt = r1
        "LDR        SP,[R1]             \n" // Load Cortex-M SP from address equals r1, i.e SP = current_pt->stack_pt
        "POP        {R4-R11}            \n" // Restore r4,r5,r6,r7,r8,r9,r10,11
        
        "CPSIE      I                   \n" // Enable global interrupts
        "BX         LR                  \n" // Return from excepction and restore r0,r1,r2,r3,r12,lr,pc,psr
    );
}

/********************************************************************************************
 *  Launches the scheduler, puts first thread into stack
 *******************************************************************************************/
void os_scheduler_launch(void)
{
    __asm volatile
    (
        "LDR        R0,=current_pt      \n" // Load address of current_pt into r0
        "LDR        R2,[R0]             \n" // Load r2 from address equals r0, r2 = current_pt
        "LDR        SP,[R2]             \n" // Load Cortex-M SP from address equals R2, i.e. SP = current_pt->stack_pt

        "POP        {R4-R11}            \n" // Restore r4,r5,r6,r7,r8,r9,r10,11
        "POP        {R0-R3}             \n" // Restore r0,r1,r2,r3
        "POP        {R12}               \n" // Restore r12

        "ADD        SP,SP,#4            \n" // Skip LR and PSR
        "POP        {LR}                \n" // Create a new start location by popping LR
        "ADD        SP,SP,#4            \n" // Skip PSR by adding 4 to SP

        "CPSIE      I                   \n" // Enable global interrupts
        "BX         LR                  \n" // Return from the exception
    );
}

/********************************************************************************************
 *  Yields a thread, resets systick and trigger and interrupt
 *******************************************************************************************/
void os_thread_yield(void)
{
    // Clear SysTick current value register
    SYSTICK->STK_VAL = 0;

    // Trigger SysTick
    INTCTRL |= PENDSTSET;
}

/********************************************************************************************
 *  Delays a thread for given time
 *******************************************************************************************/
void os_task_delay(UInt32 delay_ms)
{
    UInt32 tick_start = systick_count;
    while (systick_count - tick_start <= delay_ms)
    {
    }
}

/********************************************************************************************
 *  Returns the systick count
 *******************************************************************************************/
UInt32 os_get_tick(void)
{
    return systick_count;
}
