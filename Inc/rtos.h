/*
 * rtos.h
 *
 *  Created on: Apr 15, 2026
 *      Author: dinaliassylbek
 */

#include <stdint.h>
#include "scheduler.h"

#ifndef RTOS_H_
#define RTOS_H_

// Semaphores
struct semaphore {
	int32_t *value;
	tcbType *BlockedListHead;
};

typedef struct semaphore semaphoreType;
void OS_InitSemaphore(semaphoreType *s, int32_t initialValue);
void OS_Wait(int32_t *s);
void OS_Signal(int32_t *s);

// Thread Initialization
void SetInitialStack(int i);
int OS_AddThreads(void(*task0)(void), void(*task1)(void), void(*task2)(void));

void OS_Init(void);
void OS_Launch(uint32_t theTimeSlice);
extern void StartOS(void);

void OS_Suspend(void);
void OS_Sleep(int32_t time_ms);

#endif /* RTOS_H_ */
