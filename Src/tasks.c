/*
 * tasks.c
 * Application-level thread definitions
 * Description: Demonstrates task creation, lifecycle management, and
 * dynamic thread spawning/killing within the RTOS environment.
 *
 * Author: Dinali Assylbek
 */

//==================================================================================================
// INCLUDES
//==================================================================================================

#include "tasks.h"
#include "profile.h"
#include "rtos.h"
#include "stm32f1xx_hal.h"
#include <stdint.h>

//==================================================================================================
// GLOBAL FUNCTIONS
//==================================================================================================

void Task0(void) {

	Profile_Init(GPIOA, &PROF_PA5);
	uint32_t count = 0;

	while (1) {

		Profile_Toggle(GPIOA, GPIO_PIN_5);
		HAL_Delay(60);
		count++;

		if (count == 100) {
			OS_AddThread(Task3, 1);
		}

		if (count == 200) {
			Profile_Reset(GPIOA, GPIO_PIN_5);
			OS_KillThread();
		}

	}
}

void Task1(void) {

	Profile_Init(GPIOB, &PROF_PB2);

	while (1) {

		for (int i = 0; i < 12; i++) {
			Profile_Toggle(GPIOB, GPIO_PIN_2);
			HAL_Delay(50);
		}

		OS_Suspend();

	}
}

void Task2(void) {

	Profile_Init(GPIOC, &PROF_PC4);
	uint32_t count = 0;

	while (1) {

		Profile_Toggle(GPIOC, GPIO_PIN_4);
		count++;

		if (count % 35 == 0) {
			OS_Sleep(4500);
		} else {
			HAL_Delay(70);
		}

	}
}

void Task3(void) {

	Profile_Init(GPIOB, &PROF_PB13);

	while (1) {

		Profile_Toggle(GPIOB, GPIO_PIN_13);
		HAL_Delay(60);

	}

}
