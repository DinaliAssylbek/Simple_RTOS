/*
 * rtos.c
 *
 *  Created on: Apr 15, 2026
 *      Author: dinaliassylbek
 */

//==================================================================================================
// INCLUDES
//==================================================================================================

#include "rtos.h"
#include "bsp.h"
#include "stm32f103xb.h"
#include <stddef.h>

//==================================================================================================
// GLOBAL AND STATIC VARIABLES
//==================================================================================================

static tcbType tcbs[MAXNUMTHREADS];
static int32_t Stacks[MAXNUMTHREADS][STACKSIZE];

tcbType *RunPt;
tcbType *SleepListHead;
tcbType *ReadyListHead;

//==================================================================================================
// FUNCTION PROTOTYPES
//==================================================================================================

void OS_Scheduler(void);

/*
 * Sets up a thread's stack with dummy values so it can be "restored" the first time it runs.
 */
static void SetInitialStack(int i);

/*
 * Low-power background task that runs only when no other threads are ready.
 */
static void IdleTask(void);

/*
 * Assembly function that triggers the hardware to launch the first thread and start the OS.
 */
extern void OS_Start(void);

//==================================================================================================
// IMPLEMENTATION
//==================================================================================================

static void IdleTask(void) {
	while (1) {
		__asm volatile ("wfi"); /* assembly instruction puts the processor in a low-power state */
	}
}

static void SetInitialStack(int i) {
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

/*
 * Dynamically allocates a TCB slot and splices the new task into
 * the active circular linked list for round-robin scheduling.
 */
void OS_AddThread(void(*task)(void)) {

	uint32_t state = StartCritical();

	/* Search for the first available slot in the TCB array */
	uint32_t new_tcb_idx;
	for (new_tcb_idx = 0; new_tcb_idx < MAXNUMTHREADS; new_tcb_idx++)
	{
		if (tcbs[new_tcb_idx].status == FREE)
			break;
	}

	/* Exit if no free TCB slots are available */
	if (new_tcb_idx == MAXNUMTHREADS) {
		EndCritical(state);
		return;
	}

	/* Splice the new TCB into the circular linked list after the current running thread */
	tcbs[new_tcb_idx].next = RunPt->next;
	tcbs[new_tcb_idx].prev = RunPt;
	RunPt->next->prev = &(tcbs[new_tcb_idx]);
	RunPt->next = &(tcbs[new_tcb_idx]);

	tcbs[new_tcb_idx].status = TAKEN;	/* Mark slot as occupied */

	/* Initialize hardware context and set the entry point for the task */
	SetInitialStack(new_tcb_idx);
	Stacks[new_tcb_idx][STACKSIZE - 2] = (int32_t)(task);

	EndCritical(state);

}

/*
 * Initializes the hardware clock, clears the TCB table, and seeds the
 * circular linked list with the background IdleTask.
 */
void OS_Init(void) {

	StartCritical();

	BSP_Clock_Init();

	/* Reset all TCB statuses to ensure a clean slate for dynamic allocation */
	for(int i = 0; i < MAXNUMTHREADS; i++) {
		tcbs[i].status = FREE;
	}

	/* Initialize IdleTask (TCB 0) as the anchor for the circular list */
	tcbs[0].next = &tcbs[0];
	tcbs[0].prev = &tcbs[0];
	tcbs[0].sleep = 0;
	tcbs[0].status = TAKEN;

	/* Set the starting point for the scheduler and AddThread handshake */
	RunPt = &tcbs[0];

	/* Set up the initial stack frame for the IdleTask */
	SetInitialStack(0);
	Stacks[0][STACKSIZE - 2] = (int32_t)(&IdleTask);

}

/*
 * Configures the SysTick timer for the desired time slice and triggers
 * the assembly-level routine to start the first thread.
 */
void OS_Launch(uint32_t theTimeSlice) {
	SysTick->CTRL = 0; 						/* Disable SysTick */
	SysTick->VAL = 0; 						/* Clear Count */
	NVIC_SetPriority(SysTick_IRQn, 0x0F); 	/* Set to lowest priority */
	SysTick->LOAD = (theTimeSlice - 1); 	/* Set what count it should go up to */
	SysTick->CTRL |= 0x7; 					/* 0x07 = Enable + TickInt + ClickSource */
	OS_Start();
}

/*
 * Initializes a semaphore counter and sets up an empty queue for
 * any threads that will eventually block on this resource.
 */
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

void OS_Scheduler(void) {

	// Decrement all sleeping threads
	if (SleepListHead != NULL) {

		SleepListHead->sleep--;

		while (SleepListHead != NULL && SleepListHead->sleep == 0) {
			tcbType *p = SleepListHead;
			SleepListHead = SleepListHead->next;

			p->next = ReadyListHead;
			p->prev = ReadyListHead->prev;

			p->prev->next = p;
			p->next->prev = p;

			ReadyListHead = p;
		}

	}

	// Transition to next thread;
	RunPt = ReadyListHead;
	ReadyListHead = RunPt->next;
}
