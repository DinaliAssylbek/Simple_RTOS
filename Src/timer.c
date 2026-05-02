/*
 * timer.c
 *
 *  Created on: May 1, 2026
 *      Author: dinaliassylbek
 */

#include "timer.h"

static TIM_HandleTypeDef TIM_HandleInstance;

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim) {
	__HAL_RCC_TIM2_CLK_ENABLE();
	HAL_NVIC_SetPriority(TIM2_IRQn, TIM2_IRQn_Preempt_Priority, TIM2_IRQn_Sub_Priority);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
}

void OS_Timer_Init(void) {
	TIM_HandleInstance.Instance = TIM2;
	TIM_HandleInstance.Init.Prescaler = TIM2_IRQn_Prescaler - 1;
	TIM_HandleInstance.Init.CounterMode = TIM_COUNTERMODE_DOWN;
	TIM_HandleInstance.Init.Period = TIM2_IRQn_Period - 1;
	TIM_HandleInstance.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	HAL_TIM_Base_Init(&TIM_HandleInstance);
}

void OS_Timer_Start(void) {
	HAL_TIM_Base_Start_IT(&TIM_HandleInstance);
}

void OS_Timer_Reset(void) {
	__HAL_TIM_SET_COUNTER(&TIM_HandleInstance, 0);
}

void OS_Timer_ClearITFlag(void) {
	__HAL_TIM_CLEAR_IT(&TIM_HandleInstance, TIM_IT_UPDATE);
}
