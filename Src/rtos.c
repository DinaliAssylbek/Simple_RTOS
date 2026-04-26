/*
 * rtos.c
 *
 *  Created on: Apr 15, 2026
 *      Author: dinaliassylbek
 */

#include "rtos.h"
#include "interrupts.h"
#include "stm32f103xb.h"

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

void IdleTask(void) {
	while (1) {
		// This assembly instruction puts the processor in a low-power state
		__asm volatile ("wfi");
	}
}

int OS_AddThreads(void(*task0)(void), void(*task1)(void), void(*task2)(void)) {

	uint32_t state = StartCritical();

	// Create a linked List of tcbs
	tcbs[0].next = &tcbs[1];
	tcbs[1].next = &tcbs[2];
	tcbs[2].next = &tcbs[3];
	tcbs[3].next = &tcbs[0];

	tcbs[0].prev = &tcbs[3];
	tcbs[1].prev = &tcbs[0];
	tcbs[2].prev = &tcbs[1];
	tcbs[3].prev = &tcbs[2];

	// Initialize stacks for each thread
	SetInitialStack(0);
	SetInitialStack(1);
	SetInitialStack(2);
	SetInitialStack(3);

	// Point PC register to the functions that they should run
	Stacks[0][STACKSIZE - 2] = (int32_t)(task0);
	Stacks[1][STACKSIZE - 2] = (int32_t)(task1);
	Stacks[2][STACKSIZE - 2] = (int32_t)(task2);
	Stacks[3][STACKSIZE - 2] = (int32_t)(&IdleTask);

	// First thread to run is tcb[0]
	RunPt = &tcbs[0];
	ReadyListHead = &tcbs[1];

	EndCritical(state);

	return 1;
}

void OS_Init(void) {
	StartCritical();	// Start Critical
	BSP_Clock_Init();  // Enable System Clock
}

void OS_Launch(uint32_t theTimeSlice) {
	SysTick->CTRL = 0; // Disable SysTick
	SysTick->VAL = 0; // Clear Count
	NVIC_SetPriority(SysTick_IRQn, 0x0F); // Set to lowest priority
	SysTick->LOAD = (theTimeSlice - 1);// Set what count it should go up to
	SysTick->CTRL |= 0x7; // 0x07 = Enable + TickInt + ClickSource
	StartOS();
}

void OS_InitSemaphore(semaphoreType *s, int32_t initialValue) {
	s->value = initialValue;
	s->BlockedListHead = NULL;
	s->BlockedListTail = NULL;
}

void OS_Wait(semaphoreType *s) {
	uint32_t status = StartCritical();

	s->value--;

	// Move to blocked list
	if (s->value < 0) {

		// Remove from the Ready Linked List
		RunPt->prev->next = RunPt->next;
		RunPt->next->prev = RunPt->prev;
		ReadyListHead = RunPt->next;

		// Append to Blocked Queue
		if (s->BlockedListTail == NULL) { // Queue is empty

			s->BlockedListHead = RunPt;
			s->BlockedListTail = RunPt;
			s->BlockedListHead->next = NULL;
			s->BlockedListHead->prev = NULL;

		} else { // Queue is not empty

			s->BlockedListTail->next = RunPt;
			RunPt->prev = s->BlockedListTail;
			RunPt->next = NULL;
			s->BlockedListTail = RunPt;

		}

		OS_Suspend();

	}

	EndCritical(status);

}

void OS_Signal(semaphoreType *s) {
	uint32_t status = StartCritical();

	s->value++;

	// Wake up blocked thread
	if (s->value <= 0) {
		tcbType *p = s->BlockedListHead;
		s->BlockedListHead = s->BlockedListHead->next;

		p->next = ReadyListHead;
		p->prev = ReadyListHead->prev;

		p->prev->next = p;
		p->next->prev = p;

		ReadyListHead = p;

		if (s->BlockedListHead == NULL) {
			s->BlockedListTail = NULL;
		}
	}

	EndCritical(status);
}

void OS_Suspend(void) {
	SysTick->VAL = 0; // Clear Count
	SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk; // Trigger PendSV (Context Switch)
}

void OS_Sleep(int32_t time_ms) {

	uint32_t status = StartCritical();

	RunPt->sleep = time_ms;
	// Remove tcb from ready_queue

	RunPt->prev->next = RunPt->next;
	RunPt->next->prev = RunPt->prev;
	ReadyListHead = RunPt->next;

	if (SleepListHead == NULL) {

		SleepListHead = RunPt;
		SleepListHead->next = NULL;
		SleepListHead->prev = NULL;

	} else {

		// Edge case: time_ms < SleepPt->sleep
		if (SleepListHead->sleep >= RunPt->sleep) {

			SleepListHead->sleep -= RunPt->sleep;
			RunPt->next = SleepListHead;
			RunPt->prev = NULL;
			SleepListHead = RunPt;

		} else {

			tcbType *curr_node = SleepListHead;
			tcbType *prev_node = NULL;

			while (curr_node != NULL && curr_node->sleep < RunPt->sleep) {
				RunPt->sleep -= curr_node->sleep;
				prev_node = curr_node;
				curr_node = curr_node->next;
			}

			prev_node->next = RunPt;
			RunPt->next = curr_node;
			RunPt->prev = NULL;

			if (curr_node != NULL) {
				curr_node->sleep -= RunPt->sleep;
			}

		}

	}

	OS_Suspend();
	EndCritical(status);

}
