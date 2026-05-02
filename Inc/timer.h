/*
 * timer.h
 *
 *  Created on: May 1, 2026
 *      Author: dinaliassylbek
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "stm32f1xx_hal.h"

#define TIM2_IRQn_Preempt_Priority 0
#define TIM2_IRQn_Sub_Priority 0
#define TIM2_IRQn_Period 1000
#define TIM2_IRQn_Prescaler 72

void OS_Timer_Init(void);
void OS_Timer_Start(void);
void OS_Timer_Reset(void);
void OS_Timer_ClearITFlag(void);

#endif /* TIMER_H_ */
