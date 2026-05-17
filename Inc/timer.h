/*
 * timer.h
 * OS System Tick Hardware Driver
 * Description: Interface for the 1ms heartbeat timer (TIM2).
 *
 * Author: Dinali Assylbek
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "stm32f1xx_hal.h"

/* Timer Configuration: Based on a 72MHz clock source */
#define TIM2_IRQn_Preempt_Priority 0    /* Highest priority for scheduler precision */
#define TIM2_IRQn_Sub_Priority     0    /* Tie-breaker priority */
#define TIM2_IRQn_Period           1000000 /* 1000 ticks = 1ms at 1MHz */
#define TIM2_IRQn_Prescaler        72   /* Divides 72MHz to 1MHz */

void OS_Timer_Init(void);

void OS_Timer_Start(void);

void OS_Timer_Reset(void);

void OS_Timer_ClearITFlag(void);

#endif /* TIMER_H_ */
