/*
 * rtos.c
 *
 *  Created on: Apr 15, 2026
 *      Author: dinaliassylbek
 */

#include "rtos.h";
#include "scheduler.h";

void SetInitialStack(int i) {
	tcbs[i].sp = &Stacks[i][STACKSIZE - 16]; 	// Stack Pointer
	Stacks[i][STACKSIZE - 1] = 0x01000000; 		// Thumb bit (PCR)
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

int OS_AddThreads(void(*task0)(void), void(*task1)(void), void(*task2)(void)) {}
