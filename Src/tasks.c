/*
 * tasks.c
 *
 * Application-level thread definitions.
 * Demonstrates task creation, sleeping, and lifecycle management.
 *
 * Author: Dinali Assylbek
 */

#include "tasks.h"
#include "profile.h"
#include "rtos.h"

#include <stdint.h>

/* Toggles PA5 and dynamically spawns/kills threads based on a counter */
void Task0(void) {

	Profile_Init(GPIOA, &PROF_PA5);
	uint32_t count = 0;

	while (1) {

		Profile_Toggle(GPIOA, 5);
		HAL_Delay(60);
		count++;

		if (count == 100) {
			OS_AddThread(Task3, 1);
		}

		if (count == 200) {
			Profile_Reset(GPIOA, 5);
			OS_KillThread();
		}

	}
}

/* Toggles PB2 in a burst then suspends itself indefinitely */
void Task1(void) {

	Profile_Init(GPIOB, &PROF_PB2);

	while (1) {

		for (int i = 0; i < 12; i++) {
			Profile_Toggle(GPIOB, 2);
			HAL_Delay(50);
		}

		OS_Suspend();

	}
}

/* Toggles PC4 and demonstrates a long-duration OS_Sleep */
void Task2(void) {

	Profile_Init(GPIOC, &PROF_PC4);
	uint32_t count = 0;

	while (1) {

		Profile_Toggle(GPIOC, 4);
		count++;

		if (count % 35 == 0) {
			OS_Sleep(4500);
		} else {
			HAL_Delay(70);
		}

	}
}

/* Toggles PB13 periodically; typically spawned by Task0 */
void Task3(void) {

	Profile_Init(GPIOB, &PROF_PB13);

	while (1) {

		Profile_Toggle(GPIOB, 13);
		HAL_Delay(60);

	}

}

