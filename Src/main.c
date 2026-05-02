/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Dinali Assylbek
 * @brief          : Main program body
 ******************************************************************************
 */

#include <stdint.h>

#include "stm32f103xb.h"
#include "stm32f1xx_hal.h"
#include "tasks.h"
#include "rtos.h"

static void SystemClock_Config(void);

int main(void)
{
	HAL_Init();
	OS_Init();
	OS_AddThread(&Task0, 1);
	OS_AddThread(&Task1, 2);
	OS_AddThread(&Task2, 3);
	OS_Launch(8000); // Switch every 1ms
	return 0;
}

void panic() {

}
