/*
 * tasks.h
 *
 * Thread entry point declarations for the RTOS application.
 * All tasks follow the 'void TaskName(void)' signature.
 *
 * Author: Dinali Assylbek
 */

#ifndef TASKS_H_
#define TASKS_H_

#include "stm32f1xx_hal.h"

/**
 * Function descriptions are provided in rtos.c
 */

void Task0(void);
void Task1(void);
void Task2(void);
void Task3(void);

#endif /* TASKS_H_ */
