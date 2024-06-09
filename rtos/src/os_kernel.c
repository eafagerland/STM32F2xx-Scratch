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
#include "stm32f2xx_esl_systick.h"
#include "stm32f2xx_esl_nvic.h"

#define NUM_OF_THREADS      (3U)
#define STACK_SIZE          (100UL) // 100 * 32bits

#define BUS_FREQ            (120000000UL)

#define INTCTRL             (*((volatile UInt32 *)0xE000ED04))
#define PENDSTSET           (1U << 26U)

#define RTOS_TICKRATE_HZ    (1000UL)

static UInt32 systick_count;
static UInt32 MILLIS_PRESCALER;

struct tcb
{
    Int32 *stack_pt;
    struct tcb *next_pt;
};

typedef struct tcb tcb_type;

tcb_type tcbs[NUM_OF_THREADS];
tcb_type *current_pt;

// Each thread will have stacksize of 100 i.e. 400bytes
Int32 TCB_STACK[NUM_OF_THREADS][STACK_SIZE];

void os_scheduler_launch(void);
void os_scheduler_round_robin(void);

/********************************************************************************************
 *  Initializes the thread stacks
 *******************************************************************************************/
void os_kernel_stack_init(int i)
{
    tcbs[i].stack_pt = &TCB_STACK[i][STACK_SIZE - 16]; // Stack pointer

    TCB_STACK[i][STACK_SIZE - 1] = (1U << 24U); // Set bit21 (T-bit) in PSR to 1, to operate in Thumb mode
}

/********************************************************************************************
 *  Adds the thread to the kernel
 *******************************************************************************************/
UInt8 os_kernel_add_threads(void(*task0)(void), void(*task1)(void), void(*task2)(void))
{
    __disable_irq();

    tcbs[0].next_pt = &tcbs[1];
    tcbs[1].next_pt = &tcbs[2];
    tcbs[2].next_pt = &tcbs[0];

    // Initialize stack for thread0
    os_kernel_stack_init(0);
    // Initialize program counter (PC)
    TCB_STACK[0][STACK_SIZE - 2] = (Int32)(task0);

    // Initialize stack for thread1
    os_kernel_stack_init(1);
    // Initialize program counter (PC)
    TCB_STACK[1][STACK_SIZE - 2] = (Int32)(task1);

    // Initialize stack for thread2
    os_kernel_stack_init(2);
    // Initialize program counter (PC)
    TCB_STACK[2][STACK_SIZE - 2] = (Int32)(task2);

    // Start from thread0
    current_pt = &tcbs[0];

    __enable_irq();

    return 1;
}

/********************************************************************************************
 *  Initializes the kernel
 *******************************************************************************************/
void os_kernel_init(void)
{
    MILLIS_PRESCALER = (BUS_FREQ / RTOS_TICKRATE_HZ);
    systick_count = 0;
}

/********************************************************************************************
 *  Launches the kernel
 *******************************************************************************************/
void os_kernel_launch(void)
{
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