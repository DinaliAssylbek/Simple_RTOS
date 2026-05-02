/*
 * timer.c
 *
 *  Created on: May 1, 2026
 *      Author: dinaliassylbek
 */


#include "stm32f1xx_hal.h"

static TIM_HandleTypeDef TIM_HandleInstance;

void Timer_Init(void) {
	TIM_HandleInstance.Instance = TIM2;
	TIM_HandleInstance.Init.Prescaler = 71; // 1 MHz
	TIM_HandleInstance.Init.CounterMode = TIM_COUNTERMODE_DOWN;
	TIM_HandleInstance.Init.Period = 999;
	TIM_HandleInstance.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	HAL_TIM_Base_Init(&TIM_HandleInstance);
}
