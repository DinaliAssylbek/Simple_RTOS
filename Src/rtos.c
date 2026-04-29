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
static void setInitialStack(int i);

/*
 * Low-power background task that runs only when no other threads are ready.
 */
static void idleTask(void);

/*
 * Unlinks a TCB from its current circular list and updates the anchor
 */
static void unlink_ready(tcbType *p);

/*
 * Inserts a TCB into the circular ready list before the current Head
 */
static void link_ready(tcbType *p);

/*
 * Assembly function that triggers the hardware to launch the first thread and start the OS.
 */
extern void OS_Start(void);

//==================================================================================================
// IMPLEMENTATION
//==================================================================================================

static void idleTask(void) {
	while (1) {
		__asm volatile ("wfi"); /* assembly instruction puts the processor in a low-power state */
	}
}

static void unlink_ready(tcbType *p) {
    p->prev->next = p->next;
    p->next->prev = p->prev;
    ReadyListHead = p->next;
}

static void link_ready(tcbType *p) {
    p->next = ReadyListHead;
    p->prev = ReadyListHead->prev;
    p->prev->next = p;
    p->next->prev = p;
    ReadyListHead = p;
}

static void setInitialStack(int i) {
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

	/* Centralized insertion logic */
	link_ready(&(tcbs[new_tcb_idx]));
	tcbs[new_tcb_idx].status = TAKEN;	/* Mark slot as occupied */

	/* Initialize hardware context and set the entry point for the task */
	setInitialStack(new_tcb_idx);
	Stacks[new_tcb_idx][STACKSIZE - 2] = (int32_t)(task);

	EndCritical(state);

}

/*
 * Permanently removes the current thread from the Ready List,
 * marks its TCB as FREE for future use, and yields the CPU.
 */
void OS_KillThread(void) {

	uint32_t state = StartCritical();

	unlink_ready(RunPt);
	RunPt->status = FREE;

	/* Force an immediate context switch to a living thread */
	OS_Suspend();

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
	setInitialStack(0);
	Stacks[0][STACKSIZE - 2] = (int32_t)(&idleTask);

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

/*
 * Decrements semaphore; if < 0, moves the current thread from the
 * circular Ready List to the semaphore's FIFO Blocked Queue and yields.
 */
void OS_Wait(semaphoreType *s) {
	uint32_t status = StartCritical();

	s->value--;

	/* If value is negative, the resource is held by another thread */
	if (s->value < 0) {

		unlink_ready(RunPt);

		/* Append to Blocked Queue */
		if (s->BlockedListTail == NULL) { /* Queue is empty */

			s->BlockedListHead = RunPt;
			s->BlockedListTail = RunPt;
			s->BlockedListHead->next = NULL;
			s->BlockedListHead->prev = NULL;

		} else { 	/* Queue has threads—attach RunPt to the Tail */

			s->BlockedListTail->next = RunPt;
			RunPt->prev = s->BlockedListTail;
			RunPt->next = NULL;
			s->BlockedListTail = RunPt;

		}

		OS_Suspend();

	}

	EndCritical(status);

}

/*
 * Increments the semaphore and moves the highest-priority blocked thread
 * from the semaphore's FIFO queue back into the circular Ready List.
 */
void OS_Signal(semaphoreType *s) {
	uint32_t status = StartCritical();

	s->value++;

	/* If value is still <= 0, threads are blocked and one must be woken up */
	if (s->value <= 0) {
		/* Pop the head of the blocked FIFO queue */
		tcbType *p = s->BlockedListHead;
		s->BlockedListHead = s->BlockedListHead->next;
		link_ready(p);

		/* Maintain queue integrity if it becomes empty */
		if (s->BlockedListHead == NULL) {
			s->BlockedListTail = NULL;
		}
	}

	EndCritical(status);
}

/*
 * Manually triggers a context switch by clearing the SysTick counter
 * and setting the PendSV interrupt bit in the Interrupt Control State Register.
 */
void OS_Suspend(void) {
	/* Resets the timer count to start a fresh time slice for the next thread */
	SysTick->VAL = 0;

	/* Request PendSV exception to trigger the assembly-level context switch */
	SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
}

/*
 * Blocks the current thread for a specified time using a Delta-encoded list.
 * Removes the thread from the Ready List and inserts it into SleepListHead.
 */
void OS_Sleep(int32_t time_ms) {

	uint32_t status = StartCritical();

	RunPt->sleep = time_ms;

	unlink_ready(RunPt);

	/* Case 1: Sleep list is empty */
	if (SleepListHead == NULL) {

		SleepListHead = RunPt;
		SleepListHead->next = NULL;
		SleepListHead->prev = NULL;

	} else {

		/* Case 2: New thread should be the new head */
		if (SleepListHead->sleep >= RunPt->sleep) {

			SleepListHead->sleep -= RunPt->sleep;
			RunPt->next = SleepListHead;
			RunPt->prev = NULL;
			SleepListHead = RunPt;

		} else {

			/* Case 3: Traverse and insert while maintaining delta encoding */
			tcbType *curr_node = SleepListHead;
			tcbType *prev_node = NULL;

			while (curr_node != NULL && curr_node->sleep < RunPt->sleep) {
				RunPt->sleep -= curr_node->sleep;
				prev_node = curr_node;
				curr_node = curr_node->next;
			}

			/* Splice thread into the sleep queue */
			prev_node->next = RunPt;
			RunPt->next = curr_node;
			RunPt->prev = NULL;

			/* Update the delta for the following node, if it exists */
			if (curr_node != NULL) {
				curr_node->sleep -= RunPt->sleep;
			}

		}

	}

	OS_Suspend();
	EndCritical(status);

}

/*
 * Updates the sleep timers and determines the next thread to run.
 * Runs inside the SysTick interrupt handler.
 */
void OS_Scheduler(void) {

	/* Update Sleep Queue: only the head needs to be decremented */
	if (SleepListHead != NULL) {

		SleepListHead->sleep--;

		/* Wake all threads whose timers have expired (delta reached zero) */
		while (SleepListHead != NULL && SleepListHead->sleep == 0) {
			tcbType *p = SleepListHead;
			SleepListHead = SleepListHead->next;
			link_ready(p);
		}

	}

	/* Round-robin: Advance to the next ready thread */
	RunPt = ReadyListHead;
	ReadyListHead = RunPt->next;
}
