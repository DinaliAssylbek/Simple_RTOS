/*
 * semaphore.h
 *
 *  Created on: Apr 18, 2026
 *      Author: dinaliassylbek
 */

#include <stdint.h>

#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

void OS_InitSemaphore(int32_t *s, int32_t initialValue);
void OS_Wait(int32_t *s);
void OS_Signal(int32_t *s);

#endif /* SEMAPHORE_H_ */
