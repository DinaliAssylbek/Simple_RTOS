/*
 * timer.c
 * OS System Tick Hardware Driver
 * Description: Implementation of 1ms down-counting timer logic to
 * drive the kernel scheduler.
 *
 * Author: Dinali Assylbek
 */

//==================================================================================================
// INCLUDES
//==================================================================================================

#include "timer.h"

//==================================================================================================
// STATIC VARIABLES
//==================================================================================================

static TIM_HandleTypeDef TIM_HandleInstance;

//==================================================================================================
// GLOBAL FUNCTIONS
//==================================================================================================

/* Low-level hardware initialization for the TIM2 peripheral */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim) {

	__HAL_RCC_TIM2_CLK_ENABLE();

	/* Set interrupt priority and enable IRQ in the NVIC */
	HAL_NVIC_SetPriority(TIM2_IRQn, TIM2_IRQn_Preempt_Priority, TIM2_IRQn_Sub_Priority);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);

}

/* Configure TIM2 for a periodic 1ms heartbeat */
void OS_Timer_Init(void) {

	TIM_HandleInstance.Instance = TIM2;

	/* Clock math: (72MHz / 72) = 1MHz. (1MHz / 1000) = 1kHz (1ms) */
	TIM_HandleInstance.Init.Prescaler = TIM2_IRQn_Prescaler - 1;
	TIM_HandleInstance.Init.CounterMode = TIM_COUNTERMODE_DOWN;
	TIM_HandleInstance.Init.Period = TIM2_IRQn_Period - 1;
	TIM_HandleInstance.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

	HAL_TIM_Base_Init(&TIM_HandleInstance);

}

/* Enable timer interrupts to start the OS scheduler tick */
void OS_Timer_Start(void) {

	HAL_TIM_Base_Start_IT(&TIM_HandleInstance);

}

/* Force the counter to 0 to trigger an immediate scheduler interrupt */
void OS_Timer_Reset(void) {

	__HAL_TIM_SET_COUNTER(&TIM_HandleInstance, 0);

}

/* Clear the update interrupt flag to acknowledge the service */
void OS_Timer_ClearITFlag(void) {

	__HAL_TIM_CLEAR_IT(&TIM_HandleInstance, TIM_IT_UPDATE);

}
