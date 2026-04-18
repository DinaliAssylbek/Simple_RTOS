/*
 * interrupts.c
 *
 *  Created on: Apr 16, 2026
 *      Author: dinaliassylbek
 */

#include "interrupts.h"
#include "cmsis_gcc.h"
#include "stm32f103xb.h"

uint32_t StartCritical(void) {
	uint32_t state = __get_PRIMASK();
	__disable_irq();
	return state;
}

void EndCritical(uint32_t state) {
	__set_PRIMASK(state);
}

void BSP_Clock_Init(void) {
	RCC->CR |= RCC_CR_HSION;
	while ((RCC->CR & RCC_CR_HSIRDY) == 0);
}
