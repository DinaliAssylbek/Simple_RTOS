/*
 * rtos.h
 *
 *  Created on: Apr 15, 2026
 *      Author: dinaliassylbek
 */

#include <stdint.h>

#ifndef RTOS_H_
#define RTOS_H_

void SetInitialStack(int i);
int OS_AddThreads(void(*task0)(void), void(*task1)(void), void(*task2)(void));

void OS_Init(void);
void OS_Launch(uint32_t theTimeSlice);
extern void StartOS(void);

#endif /* RTOS_H_ */
