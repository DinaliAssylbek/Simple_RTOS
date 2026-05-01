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

	Profile_Init(&PROF_PA5);
	uint32_t count = 0;

	while (1) {

		Profile_Toggle(&PROF_PA5);
		OS_Sleep(60);
		count++;

		if (count == 100) {
			OS_AddThread(Task3, 1);
		}

		if (count == 200) {
			Profile_Reset(&PROF_PA5);
			OS_KillThread();
		}

	}
}

/* Toggles PB2 in a burst then suspends itself indefinitely */
void Task1(void) {

	Profile_Init(&PROF_PB2);

	while (1) {

		for (int i = 0; i < 12; i++) {
			Profile_Toggle(&PROF_PB2);
			OS_Sleep(50);
		}

		OS_Suspend();

	}
}

/* Toggles PC4 and demonstrates a long-duration OS_Sleep */
void Task2(void) {

	Profile_Init(&PROF_PC4);
	uint32_t count = 0;

	while (1) {

		Profile_Toggle(&PROF_PC4);
		count++;

		if (count % 35 == 0) {
			OS_Sleep(4500);
		} else {
			OS_Sleep(70);
		}

	}
}

/* Toggles PB13 periodically; typically spawned by Task0 */
void Task3(void) {

	Profile_Init(&PROF_PB13);

	while (1) {

		Profile_Toggle(&PROF_PB13);
		OS_Sleep(60);

	}

}

