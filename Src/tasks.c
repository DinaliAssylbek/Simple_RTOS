/*
 * tasks.c
 *
 *  Created on: Apr 29, 2026
 *      Author: dinaliassylbek
 */

#include <stdint.h>
#include "tasks.h"
#include "profile.h"
#include "rtos.h"

void task0(void) {
	Profile_InitPA5();
	uint32_t count = 0;
	while (1) {
		Profile_TogglePA5();
		count++;
		if (count == 100) {
			OS_AddThread(&task3, 1);
		}
		if (count == 200) {
			Profile_ResetPA5();
			OS_KillThread();
		}
	}
}

void task1(void) {
	Profile_InitPB2();
	while (1) {
		for (int i = 0; i < 12; i++) {
			Profile_TogglePB2();
			// Delay
		}

		OS_Suspend();
	}
}

void task2(void) {
	Profile_InitPC4();
	uint32_t count = 0;
	while (1) {
		Profile_TogglePC4();
		count++;
		if (count % 35 == 0) {
			OS_Sleep(4500);
		} else {
			// delay
		}
	}
}

void task3(void) {
	Profile_InitPB13();
	while (1) {
		Profile_TogglePB13();
		// delay
	}
}

