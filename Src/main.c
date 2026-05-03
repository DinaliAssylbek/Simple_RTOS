/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Dinali Assylbek
 * @brief          : Main program body
 ******************************************************************************
 */

#include "tasks.h"
#include "rtos.h"

int main(void)
{
	HAL_Init();
	OS_Init();

	OS_AddThread(&Task0, 1);
	OS_AddThread(&Task1, 2);
	OS_AddThread(&Task2, 3);
	OS_Launch(); // Switch every 1ms
	return 0;
}

void panic() {

}
