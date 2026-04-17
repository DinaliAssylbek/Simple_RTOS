/*
 * rtos.c
 *
 *  Created on: Apr 15, 2026
 *      Author: dinaliassylbek
 */

#include "rtos.h"
#include "scheduler.h"
#include "interrupts.h"

void SetInitialStack(int i) {
	tcbs[i].sp = &Stacks[i][STACKSIZE - 16]; 	// Stack Pointer
	Stacks[i][STACKSIZE - 1] = 0x01000000; 		// Thumb bit (PSR)
	Stacks[i][STACKSIZE - 3] = 0x10101010; 		// Link Register (R14)
	Stacks[i][STACKSIZE - 4] = 0x12121212; 		// R12
	Stacks[i][STACKSIZE - 5] = 0x20202020; 		// R3
	Stacks[i][STACKSIZE - 6] = 0x21212121; 		// R2
	Stacks[i][STACKSIZE - 7] = 0x02020202; 		// R1
	Stacks[i][STACKSIZE - 8] = 0x32323232; 		// R0
	Stacks[i][STACKSIZE - 9] = 0x31313131; 		// R11
	Stacks[i][STACKSIZE - 10] = 0x30303030;		// R10
	Stacks[i][STACKSIZE - 11] = 0x03030303;		// R9
	Stacks[i][STACKSIZE - 12] = 0x13131313;		// R8
	Stacks[i][STACKSIZE - 13] = 0x23232323;		// R7
	Stacks[i][STACKSIZE - 14] = 0x04040404;		// R6
	Stacks[i][STACKSIZE - 15] = 0x40404040;		// R5
	Stacks[i][STACKSIZE - 16] = 0x41414141;		// R4
}

int OS_AddThreads(void(*task0)(void), void(*task1)(void), void(*task2)(void)) {

	uint32_t state = StartCritical();

	// Create a linked List of tcbs
	tcbs[0].next = &tcbs[1];
	tcbs[1].next = &tcbs[2];
	tcbs[2].next = &tcbs[0];

	// Initialize stacks for each thread
	SetInitialStack(0);
	SetInitialStack(1);
	SetInitialStack(2);

	// Point PC register to the functions that they should run
	Stacks[0][STACKSIZE - 2] = (int32_t)(task0);
	Stacks[1][STACKSIZE - 2] = (int32_t)(task1);
	Stacks[2][STACKSIZE - 2] = (int32_t)(task2);

	// First thread to run is tcb[0]
	RunPt = &tcbs[0];

	EndCritical(state);

	return 1;

}

void OS_Init(void) {

}

void OS_Launch(uint32_t theTimeSlice) {

}

