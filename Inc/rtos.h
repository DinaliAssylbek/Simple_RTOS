/**
 * rtos.h
 *
 * The module os encapsulates the core functionality of the operating system and
 * exposes the functions for interacting with it.
 *
 * Author: Dinali Assylbek
 */

#ifndef RTOS_H_
#define RTOS_H_

#include <stdint.h>

#define STACKSIZE 100	/* Number of 32-bit words in each TCB's stack */
#define NUMTHREADS 4	/* Maximum number of threads the OS can manage */

/* Thread Control Block: Tracks thread state, stack, and list links */
typedef struct tcb {
    int32_t *sp;        /* Pointer to the current top of stack (saved during context switch) */
    int32_t sleep;      /* Remaining time slices for the thread to stay in sleep state */
    struct tcb *next;   /* Pointer to the next TCB in the circular ready or blocked list */
    struct tcb *prev;   /* Pointer to the previous TCB in the circular ready or blocked list */
} tcbType;

/* Counting Semaphore: Manages synchronization and blocked thread queue */
typedef struct semaphore {
    int32_t value;              /* Current value of the semaphore (negative indicates blocked threads) */
    tcbType *BlockedListHead;   /* Head of the FIFO queue containing threads blocked on this semaphore */
    tcbType *BlockedListTail;   /* Tail of the FIFO queue containing threads blocked on this semaphore */
} semaphoreType;

/**
 * Function descriptions are provided in rtos.c
 */

void OS_Init(void);

void OS_AddThreads(void(*task0)(void), void(*task1)(void), void(*task2)(void));

void OS_Launch(uint32_t theTimeSlice);

void OS_InitSemaphore(semaphoreType *s, int32_t initialValue);

void OS_Scheduler(void);

void OS_Suspend(void);

void OS_Sleep(int32_t time_ms);

void OS_Wait(semaphoreType *s);

void OS_Signal(semaphoreType *s);

#endif /* RTOS_H_ */
