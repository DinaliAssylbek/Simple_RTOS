/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Dinali Assylbek
 * @brief          : Main program body
 ******************************************************************************
 */

#include "tasks.h"
#include "rtos.h"
#include "bsp.h"
#include "profile.h"

int main(void)
{
	HAL_Init();
	SystemClock_Config();
	OS_Init();

	OS_AddThread(&Task0, 1);
	OS_AddThread(&Task1, 1);
	OS_AddThread(&Task2, 1);
	OS_Launch(); // Switch every 1ms
	return 0;
}

void panic() {
	StartCritical();
	__asm("BKPT 1");
}
