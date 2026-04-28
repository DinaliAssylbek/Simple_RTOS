/*
 * bsp.c
 * Board Support Package for STM32F103
 * Description: Low-level hardware abstraction layer providing atomic
 * critical sections and system clock initialization.
 *
 * Author: Dinali Assylbek
 */

//==================================================================================================
// INCLUDES
//==================================================================================================

#include "bsp.h"
#include "cmsis_gcc.h"
#include "stm32f103xb.h"

//==================================================================================================
// GLOBAL FUNCTIONS
//==================================================================================================

uint32_t StartCritical(void) {
	uint32_t state = __get_PRIMASK();	/* Save current interrupt mask state */
	__disable_irq();					/* Disable all configurable interrupts */
	return state;
}

void EndCritical(uint32_t state) {
	__set_PRIMASK(state);				/* Restore mask state to re-enable interrupts if necessary */
}

void BSP_Clock_Init(void) {
	RCC->CR |= RCC_CR_HSION;					/* Enable High Speed Internal (HSI) oscillator */
	while ((RCC->CR & RCC_CR_HSIRDY) == 0);		/* Wait until HSI is stable and ready to use */
}
