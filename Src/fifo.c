/*
 * fifo.c
 *
 *  Created on: Apr 19, 2026
 *      Author: dinaliassylbek
 */

#include "fifo.h"
#include "rtos.h"

int32_t static Fifo[FIFOSIZE];
int32_t volatile *GetPt;
int32_t volatile *PutPt;

int32_t CurrentSize;
int32_t RoomLeft;
int32_t FIFOmutex;

void OS_Fifo_Init(void) {
	GetPt = &Fifo[0];
	PutPt = &Fifo[0];

	OS_InitSemaphore(&CurrentSize, 0);
	OS_InitSemaphore(&RoomLeft, FIFOSIZE);
	OS_InitSemaphore(&FIFOmutex, 1);
}

void OS_Fifo_Put(int32_t data) {
	OS_Wait(&RoomLeft);
	OS_Wait(&FIFOmutex);

	*PutPt = data;
	PutPt++;

	if (PutPt == &Fifo[FIFOSIZE]) {
		PutPt = &Fifo[0];
	}

	OS_Signal(&FIFOmutex);
	OS_Signal(&CurrentSize);
}

int32_t OS_Fifo_Get(void) {
	OS_Wait(&CurrentSize);
	OS_Wait(&FIFOmutex);

	int32_t data = *GetPt;
	GetPt++;

	if (GetPt == &Fifo[FIFOSIZE]) {
		GetPt = &Fifo[0];
	}

	OS_Signal(&FIFOmutex);
	OS_Signal(&RoomLeft);

	return data;
}
