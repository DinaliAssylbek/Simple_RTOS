/**
 * rtos.c
 * Author: Dinali Assylbek
 * Date: May 2026
 */

#include "rtos.h"
#include "bsp.h"
#include "timer.h"
#include "stm32f103xb.h"
#include <stddef.h>

//==============================================================================
// KERNEL STORAGE
//==============================================================================

static tcbType  tcbs[MAXNUMTHREADS];
static int32_t  Stacks[MAXNUMTHREADS][STACKSIZE];

tcbType *RunPt;
tcbType *SleepListHead;

//==============================================================================
// INTERNAL PROTOTYPES
//==============================================================================

void OS_Scheduler(void);
static void setInitialStack(int i);
static void idleTask(void);
static void unlink_ready(tcbType *p);
static void link_ready(tcbType *p);

extern void OS_Start(void);
extern void OS_ThreadSwitch(void);

//==============================================================================
// HELPERS
//==============================================================================

/* Puts the CPU in low-power mode when no threads are ready */
static void idleTask(void) {
    while (1) {
        __asm volatile ("wfi");
    }
}

/* Removes a thread from the circular ready list */
static void unlink_ready(tcbType *p) {
    p->prev->next = p->next;
    p->next->prev = p->prev;
}

/* Adds a thread into the circular ready list after RunPt */
static void link_ready(tcbType *p) {
    p->next = RunPt->next;
    p->prev = RunPt;
    RunPt->next->prev = p;
    RunPt->next = p;
}

/* Forges a hardware-compatible stack frame for new threads */
static void setInitialStack(int i) {
    tcbs[i].sp = &Stacks[i][STACKSIZE - 16];

    Stacks[i][STACKSIZE - 1]  = 0x01000000; // xPSR (Thumb bit)
    Stacks[i][STACKSIZE - 3]  = 0x10101010; // R14 (LR)
    Stacks[i][STACKSIZE - 4]  = 0x12121212; // R12
    Stacks[i][STACKSIZE - 5]  = 0x20202020; // R3
    Stacks[i][STACKSIZE - 6]  = 0x21212121; // R2
    Stacks[i][STACKSIZE - 7]  = 0x02020202; // R1
    Stacks[i][STACKSIZE - 8]  = 0x32323232; // R0

    /* These registers must be restored manually in assembly */
    Stacks[i][STACKSIZE - 9]  = 0x31313131; // R11
    Stacks[i][STACKSIZE - 10] = 0x30303030; // R10
    Stacks[i][STACKSIZE - 11] = 0x03030303; // R9
    Stacks[i][STACKSIZE - 12] = 0x13131313; // R8
    Stacks[i][STACKSIZE - 13] = 0x23232323; // R7
    Stacks[i][STACKSIZE - 14] = 0x04040404; // R6
    Stacks[i][STACKSIZE - 15] = 0x40404040; // R5
    Stacks[i][STACKSIZE - 16] = 0x41414141; // R4
}

//==============================================================================
// KERNEL API
//==============================================================================

/* Prepares the kernel, timer, and the background idle task */
void OS_Init(void) {
    StartCritical();
    OS_Timer_Init();

    for (int i = 0; i < MAXNUMTHREADS; i++) {
        tcbs[i].status = FREE;
    }

    /* Initialize the idle task as the first node in the circular list */
    tcbs[0].next     = &tcbs[0];
    tcbs[0].prev     = &tcbs[0];
    tcbs[0].sleep    = 0;
    tcbs[0].status   = TAKEN;
    tcbs[0].priority = UINT8_MAX;

    setInitialStack(0);
    Stacks[0][STACKSIZE - 2] = (int32_t)(&idleTask);

    RunPt = &tcbs[0];
    SleepListHead = NULL;
}

/* Allocates a TCB and adds a new function to the scheduler */
void OS_AddThread(void(*task)(void), uint8_t priority) {
    uint32_t state = StartCritical();

    uint32_t new_idx;
    for (new_idx = 0; new_idx < MAXNUMTHREADS; new_idx++) {
        if (tcbs[new_idx].status == FREE) break;
    }

    assert_or_panic(new_idx != MAXNUMTHREADS);

    tcbs[new_idx].priority = priority;
    tcbs[new_idx].status   = TAKEN;

    setInitialStack(new_idx);
    Stacks[new_idx][STACKSIZE - 2] = (int32_t)(task);

    link_ready(&(tcbs[new_idx]));
    EndCritical(state);
}

/* Removes the active thread from rotation and yields the CPU */
void OS_KillThread(void) {
    uint32_t state = StartCritical();
    if (RunPt == &tcbs[0]) panic(); // Cannot kill the idle task

    RunPt->status = FREE;
    unlink_ready(RunPt);

    OS_Suspend(); // Force switch since RunPt is no longer valid
    EndCritical(state);
}

/* Starts the hardware timer and triggers the first thread */
void OS_Launch(void) {
    OS_Timer_Start();
    OS_Start();
}

/* Triggers a PendSV exception to initiate a context switch */
void OS_Suspend(void) {
    OS_Timer_Reset();
}

//==============================================================================
// SYNCHRONIZATION
//==============================================================================

/* Initializes a counting semaphore with an empty block list */
void OS_InitSemaphore(semaphoreType *s, int32_t initialValue) {
    s->value = initialValue;
    s->BlockedListHead = NULL;
    s->BlockedListTail = NULL;
}

/* Acquires a resource or blocks the thread if none are available */
void OS_Wait(semaphoreType *s) {
    uint32_t status = StartCritical();
    s->value--;

    if (s->value < 0) {
        tcbType *curr = RunPt;

        /* Must point RunPt elsewhere before unlinking it from ready list */
        RunPt = curr->next;
        unlink_ready(curr);

        /* Standard FIFO queue insertion */
        if (s->BlockedListTail == NULL) {
            s->BlockedListHead = curr;
            s->BlockedListTail = curr;
            curr->next = NULL;
            curr->prev = NULL;
        } else {
            s->BlockedListTail->next = curr;
            curr->prev = s->BlockedListTail;
            curr->next = NULL;
            s->BlockedListTail = curr;
        }
        OS_Suspend();
    }
    EndCritical(status);
}

/* Releases a resource and wakes the first waiting thread */
void OS_Signal(semaphoreType *s) {
    uint32_t status = StartCritical();
    s->value++;

    if (s->value <= 0) {
        /* Pop from FIFO blocked list */
        tcbType *p = s->BlockedListHead;
        s->BlockedListHead = p->next;

        if (s->BlockedListHead == NULL) {
            s->BlockedListTail = NULL;
        } else {
            s->BlockedListHead->prev = NULL;
        }
        link_ready(p); // Move back to the ready rotation
    }
    EndCritical(status);
}

//==============================================================================
// SLEEP
//==============================================================================

/* Blocks the current thread for time_ms using a delta-encoded list */
void OS_Sleep(int32_t time_ms) {
    uint32_t status = StartCritical();

    RunPt->sleep = time_ms;
    tcbType *curr = RunPt;

    /* Ensure RunPt is safe before removing this thread from ready list */
    RunPt = curr->next;
    unlink_ready(curr);

    if (SleepListHead == NULL) {
        SleepListHead = curr;
        curr->next = NULL;
        curr->prev = NULL;
    } else {
        /* New thread wakes up before the current head */
        if (SleepListHead->sleep >= curr->sleep) {
            SleepListHead->sleep -= curr->sleep; // Adjust next delay relative to new head
            curr->next = SleepListHead;
            SleepListHead->prev = curr;
            curr->prev = NULL;
            SleepListHead = curr;
        } else {
            /* Find the correct spot while subtracting elapsed deltas */
            tcbType *curr_node = SleepListHead;
            tcbType *prev_node = NULL;

            while (curr_node != NULL && curr_node->sleep < curr->sleep) {
                curr->sleep -= curr_node->sleep;
                prev_node = curr_node;
                curr_node = curr_node->next;
            }

            prev_node->next = curr;
            curr->next = curr_node;
            curr->prev = prev_node;

            /* Adjust the delta of the following thread */
            if (curr_node != NULL) {
                curr_node->prev = curr;
                curr_node->sleep -= curr->sleep;
            }
        }
    }
    OS_Suspend();
    EndCritical(status);
}

//==============================================================================
// SCHEDULER
//==============================================================================

/* Decides which thread to run next based on priority and sleep status */
void OS_Scheduler(void) {
    OS_Timer_ClearITFlag();

    /* 1. Update delta sleep list; only the head needs a decrement */
    if (SleepListHead != NULL) {
        SleepListHead->sleep--;

        /* Wake all threads whose delta has reached zero */
        while (SleepListHead != NULL && SleepListHead->sleep == 0) {
            tcbType *p = SleepListHead;
            SleepListHead = SleepListHead->next;
            if (SleepListHead != NULL) SleepListHead->prev = NULL;
            link_ready(p);
        }
    }

    /* 2. Priority Search: starts at RunPt->next to support Round-Robin */
    tcbType *next_pt = RunPt->next;
    tcbType *iterating_pt = next_pt;
    uint32_t max_priority = 256;
    tcbType *best_pt = next_pt;

    do {
        if (iterating_pt->priority < max_priority) {
            best_pt = iterating_pt;
            max_priority = best_pt->priority;
        }
        iterating_pt = iterating_pt->next;
    } while (iterating_pt != next_pt);

    RunPt = best_pt;
}
