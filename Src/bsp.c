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

void SystemClock_Config(void) {

	RCC_OscInitTypeDef RCC_OscInstance = {0};
	RCC_OscInstance.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInstance.HSEState = RCC_HSE_ON;
	RCC_OscInstance.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	RCC_OscInstance.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInstance.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInstance.PLL.PLLMUL = RCC_PLL_MUL9;
	HAL_RCC_OscConfig(&RCC_OscInstance);

	RCC_ClkInitTypeDef RCC_ClkInstance = {0};
	RCC_ClkInstance.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInstance.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInstance.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInstance.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInstance.APB2CLKDivider = RCC_SYSCLK_DIV1;
	HAL_RCC_ClockConfig(&RCC_ClkInstance, FLASH_LATENCY_0);
}
