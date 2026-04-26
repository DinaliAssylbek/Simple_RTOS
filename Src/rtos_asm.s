	.syntax unified
	.thumb

	.global StartOS
	.global SysTick_Handler

	.extern RunPt
	.extern OS_Scheduler

	.section .text

// =====================
// StartOS
// =====================
.type StartOS, %function
StartOS:
	LDR R0, =RunPt	 // Get Address of the RunPtr Pointer
	LDR R1, [R0]	 // Get the Address of TCB (Pointing to the Stack Pointer)
	LDR SP, [R1]	 // Load Stack Pointer value into SP
	POP {R4-R11}	 // Pop PendSV registers
	POP {R0-R3}		 // Pop Registers R0 through R3
	POP {R12}		 // Pop R12 Register
	ADD SP, SP, #4	 // Skip LR Register value
	POP {LR}		 // Put function address (PC) into LR
	ADD SP, SP, #4	 // Skip PCR register
	CPSIE I			 // Enable Interrupts
	BX LR			 // Return to first function (task0)

// =====================
// SysTick_Handler
// =====================
.type SysTick_Handler, %function
SysTick_Handler:
	CPSID I			 // Disable Interrupts
	PUSH {R4-R11}	 // push remaining registers onto the stack

	LDR R0, =RunPt	 // Get Address of the RunPtr Pointer
	LDR R1, [R0]	 // Get the Address of TCB (Pointing to the Stack Pointer)
	STR SP, [R1]	 // Save the new stack pointer in the tcb

    PUSH {R0, LR}	 // Save R0 and LR on stack for later use
    BL OS_Scheduler	 // Run C implementation of Scheduler
    POP {R0, LR}	 // Retrieve R0 and LR from stack
    LDR R1, [R0]	 // R1 = RunPt, new thread

	LDR SP, [R1]	 // Load Stack Pointer value into SP
	POP {R4-R11}	 // Pop PendSV registers

	CPSIE I			 // Enable Interrupts
	BX LR			 // Return to next function
