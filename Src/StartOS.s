	.syntax unified
	.global StartOS
	.thumb

	.extern RunPt
	.section .text

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
