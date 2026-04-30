/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Dinali Assylbek
 * @brief          : Main program body
 ******************************************************************************
 */

#include <stdint.h>
#include "stm32f103xb.h"
#include "tasks.h"
#include "rtos.h"

int main(void)
{
	OS_Init();
	OS_AddThread(&task0, 1);
	OS_AddThread(&task1, 2);
	OS_AddThread(&task2, 3);
	OS_Launch(8000); // Switch every 1ms
	return 0;
}
