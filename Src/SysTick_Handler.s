	.syntax unified
	.global SysTick_Handler
	.thumb

	.extern RunPt
	.extern Scheduler
	.section .text

.type SysTick_Handler, %function
SysTick_Handler:
	CPSID I			 // Disable Interrupts
	PUSH {R4-R11}	 // push remaining registers onto the stack

	LDR R0, =RunPt	 // Get Address of the RunPtr Pointer
	LDR R1, [R0]	 // Get the Address of TCB (Pointing to the Stack Pointer)
	STR SP, [R1]	 // Save the new stack pointer in the tcb

    PUSH {R0, LR}	 // Save R0 and LR on stack for later use
    BL Scheduler	 // Run C implementation of Scheduler
    POP {R0, LR}	 // Retrieve R0 and LR from stack
    LDR R1, [R0]	 // R1 = RunPt, new thread

	LDR SP, [R1]	 // Load Stack Pointer value into SP
	POP {R4-R11}	 // Pop PendSV registers

	CPSIE I			 // Enable Interrupts
	BX LR			 // Return to next function
