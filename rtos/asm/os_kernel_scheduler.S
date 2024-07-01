.cpu cortex-m3
.syntax unified
.thumb

// Define global variables
.section .data
    .align 4

.global systick_count
.global current_pt
.global active_thread
.global thread_count
.global thread_array
.global idle_thread_handler

// Init local variables
.section .bss
    .align 4

thread_index:
    .space 4

highest_pri_index:
    .space 4

highest_pri:
    .space 4

previous_thread_index:
    .space 4

num_threads_top_pri:
    .space 4

original_lr:
    .space 4

.section .text
.global os_scheduler_launch
.type os_scheduler_launch, %function

.global ESL_SysTick_Handler
.type ESL_SysTick_Handler, %function

.global os_idle_hook
.type os_idle_hook, %function

/********************************************************************************************
 *  Launches the scheduler, puts first thread into stack
 *******************************************************************************************/
os_scheduler_launch:
    ldr        r0,=current_pt       // Load address of current_pt into r0
    ldr        r2,[r0]              // Load r2 from address equals r0, r2 = current_pt
    ldr        sp,[r2]              // Load Cortex-M SP from address equals R2, i.e. SP = current_pt->stack_pt

    pop        {r4-r11}             // Restore r4,r5,r6,r7,r8,r9,r10,11
    pop        {r0-r3}              // Restore r0,r1,r2,r3
    pop        {r12}                // Restore r12

    add        sp,sp,#4             // Skip LR and PSR
    pop        {lr}                 // Create a new start location by popping LR
    add        sp,sp,#4             // Skip PSR by adding 4 to SP

    cpsie      i                    // Enable global interrupts
    bx         lr                   // Return from the exception

/********************************************************************************************
 *  Systick interrupt handler. Increments the systick counter.
 *******************************************************************************************/
// When exception occurs these registers are automatically saved onto the stack: r0,r1,r2,r3,r12,lr,pc,psr
ESL_SysTick_Handler:
    cpsid       i                   // Disable interrupts

    // Set num_threads_top_pri back to 0
    ldr     r0, =num_threads_top_pri   // Load address of num_threads_top_pri into r3
    mov     r1, #0                     // Move 0 into r2
    str     r1, [r0]                   // Store 0 at the address in r3

    // Set thread_index back to 0
    //ldr     r0, =thread_index   // Load address of num_threads_top_pri into r3
    //mov     r1, #0                     // Move 0 into r2
    //str     r1, [r0]                   // Store 0 at the address in r3

    // Increment SysTick Counter    
    ldr         r0,=systick_count   // Load address of systick count into R0
    ldr         r0,[r0]             // Load value of systick_count into R0
    add         r0,r0,#1            // Increment systick_count

    ldr         r1,=systick_count   // Load address of systick_count into R1
    str         r0,[r1]             // Store incremented value back to systick_count

    // SUSPEND CURRENT THREAD   
    push        {r4-r11}   

    ldr         r0,=current_pt      // Load address of current_pt into r0
    ldr         r1,[r0]             // Load r1 from address equals r0, i.e. r1 = current_pt
    str         sp,[r1]             // Store SP into R1 ie current_pt

    // Iterate all threads to get the one with highest priority that doesnt now have a wakup tick pending and
    // is not suspended.

    mov         r0, #0              // Init r0 with value 0

loop_start:
    ldr         r1, =thread_count   // Load address of thread_count into r1
    ldr         r1, [r1]            // Load value of thread_count into r1
    cmp         r0, r1              // Compare r0 with the thread_count(r1)
    bge         loop_end            // Branch to loop_end if r0 >= thread_count

    // Prepare offset to get current thread
    mov         r2, #20             // Load the 20byte offset for next thread in thread array into R2
    mul         r2, r0, r2          // Multiply the loop counter with offset

    // Get the thread from the array by adding the offset * iterator
    ldr         r3,=thread_array    // Load address of thread_array into r0
    ldr         r3,[r3]             // Load value of thread array
    add         r2, r3, r2          // Add R2 to R

    // Check if wakeup is pending and decrement if needed
    ldr         r1, [r2, #16]       // Get the tick_wakeup value (16 bytes offset in tcb struct)
    cmp         r1, #0              // Compare r1 with 0
    ble         skip_decrement      // Branch if less or equal to 0
    sub         r1, r1, #1          // Decrement r1 by 1
    str         r1, [r2, #16]       // Store decremented value back to tick_wakeup
    b           thread_suspended    // Thread is suspended and will be skipped
skip_decrement:

    // Get priority value and get highest from all threads
    ldr         r1, [r2, #8]                // Load thread priority into r1
    ldr         r3,=highest_pri             // Load address of current highest priority 
    ldr         r3, [r3]                    // Load value of current highest priority
    cmp         r1, r3                      // Compare this threads priority with current highest one
    bgt         increased_priority          // Branch if current threads priority is higher
    ldr         r3, =num_threads_top_pri    // Load address of num_threads_top_pri into r3
    ldr         r2, [r3]                    // Load value of num_threads_top_pri into r3
    add         r2, r2, #1                  // Increment r2 by 1
    str         r2, [r3]                    // Store incremented value back
    b           priority_not_changed

increased_priority:
    ldr         r3, =num_threads_top_pri    // Load address of num_threads_top_pri into r3
    mov         r2, #1                      // Move 1 into r2
    str         r2, [r3]                    // Store 1 at the address in r3
    ldr         r3,=highest_pri             // Load address of current highest priority 
    str         r1, [r3]                    // Store current highest prio at the address in r3

priority_not_changed:

thread_suspended:

    add         r0, r0, #1          // Increment r0
    b           loop_start          // Branch to loop_start

loop_end:
    // Enable the next pending thread, idle hook if no threads active
    ldr         r0, =num_threads_top_pri    // Load address of num_threads_top_pri into r3
    ldr         r0, [r0]                    // Load value of num_threads_top_pri into r3
    cmp         r0, #0
    ble         idle_hook                   // Branch if num_threads 0


    // Get next thread  
    ldr         r1,=thread_index    // Load address of active_thread into R1
    ldr         r1,[r1]             // Load value of active_thread into R1
    add         r1,r1,#1            // Increment active thread index
    ldr         r2,=thread_count    // Load address of thread count into R2
    ldr         r2,[r2]             // Load value of thread count   
    cmp         r1, r2              // check if index is within num of threads  
    blt         increment_thread    

    // Reset active thread index if equal thread count
    mov         r2, #0              // Load 0 into R2
    ldr         r1,=thread_index    // Load address of active_thread into R1
    str         r2,[r1]             // Set active_thread to 0
    b           next_thread  

increment_thread:               
    ldr         r2,=thread_index    // load address of thread index into R2
    str         r1,[r2]             // Store the Incremented in

next_thread:                    
    mov         r2, #20             // Load the 16byte offset for next thread in thread array into R2
    ldr         r1,=thread_index    // Load address of active_thread into R1
    ldr         r1,[r1]             // Load value of active_thread into R1
    mul         r2, r1, r2          // Multiply the loop counter with R1(active_thread)

    ldr         r3,=thread_array    // Load address of current_pt into r0
    ldr         r3,[r3]             // Load value of thread array
    add         r2, r3, r2          // Add R2 to R

    ldr         r0,=current_pt      // Load address of current_pt into r0
    str         r2,[r0]             // Store r2 at address equal r0, i.e current_pt = r2
    ldr         sp,[r2]             // Load Cortex-M SP from address equals r2, i.e SP = thread_array[thread index]
    b           skip_idle_hook

// If no threads are pending, switch to idle thread
idle_hook:
    ldr         r0, =idle_thread_handler    // Load address of idle_thread_handler into r0
    ldr         r3, [r0, #16]               // Load tick_wakeup into r3
    add         r3, r3, #1                  // Increment tick_wakeup by 1
    str         r3, [r0, #16]               // Store incremented tick_wakup back into thread
    ldr         r1, =current_pt             // Load address of current_pt into r1
    str         r0, [r1]                    // Store idle thread as current_pt
    ldr         sp, [r0]                    // Load idle thread stack pointer

skip_idle_hook:

    pop         {r4-r11}
    cpsie       i                   // Enable global interrupts

    bx          lr                  // Return from excepction and restore r0,r1,r2,r3,r12,lr,pc,psr
