	.syntax unified
	.global SysTick_Handler
	.thumb

	.extern RunPt
	.section .text

.type SysTick_Handler, %function
SysTick_Handler:
	CPSID I			 // Disable Interrupts
	PUSH {R4-R11}	 // push remaining registers onto the stack

	LDR R0, =RunPt	 // Get Address of the RunPtr Pointer
	LDR R1, [R0]	 // Get the Address of TCB (Pointing to the Stack Pointer)
	STR SP, [R1]	 // Save the new stack pointer in the tcb

	LDR R1, [R1, #4] // Load the 'next' TCB address from offset 4
    STR R1, [R0]     // Update RunPt memory with the new TCB address

	LDR SP, [R1]	 // Load Stack Pointer value into SP
	POP {R4-R11}	 // Pop PendSV registers

	CPSIE I			 // Enable Interrupts
	BX LR			 // Return to next function
