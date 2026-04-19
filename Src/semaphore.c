/*
 * semaphore.c
 *
 *  Created on: Apr 18, 2026
 *      Author: dinaliassylbek
 */


#include "semaphore.h"
#include "interrupts.h"

void OS_InitSemaphore(int32_t *s, int32_t initialValue) {
	(*s) = initialValue;
}

void OS_Wait(int32_t *s) {
	uint32_t status = StartCritical();
	while (*s == 0) {
		EndCritical(status);
		status = StartCritical();
	}
	(*s) = (*s) - 1;
	EndCritical(status);
}

void OS_Signal(int32_t *s) {
	uint32_t status = StartCritical();
	(*s) = (*s) + 1;
	EndCritical(status);
}
