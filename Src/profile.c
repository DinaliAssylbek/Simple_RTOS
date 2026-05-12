/*
 * profile.c
 * High-Speed Logic Profiling Tools
 * Description: Provides low-latency GPIO manipulation for timing analysis
 * and task execution monitoring on the STM32F103.
 *
 * Author: Dinali Assylbek
 */

//==================================================================================================
// INCLUDES
//==================================================================================================

#include "profile.h"

//==================================================================================================
// CONFIGURATIONS
//==================================================================================================

const GPIO_InitTypeDef PROF_PA5  = {GPIO_PIN_5,  GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW};
const GPIO_InitTypeDef PROF_PB2  = {GPIO_PIN_2,  GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW};
const GPIO_InitTypeDef PROF_PB13 = {GPIO_PIN_13, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW};
const GPIO_InitTypeDef PROF_PC4  = {GPIO_PIN_4,  GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW};

//==================================================================================================
// GLOBAL FUNCTIONS
//==================================================================================================

void Profile_Init(GPIO_TypeDef *GPIOx, const GPIO_InitTypeDef *GPIO_Init) {

	/* Enable the Clock for the specific port */
	if      (GPIOx == GPIOA) __HAL_RCC_GPIOA_CLK_ENABLE();
	else if (GPIOx == GPIOB) __HAL_RCC_GPIOB_CLK_ENABLE();
	else if (GPIOx == GPIOC) __HAL_RCC_GPIOC_CLK_ENABLE();

	/* Initialize the pin. Cast to (GPIO_InitTypeDef*) to satisfy HAL signature */
	HAL_GPIO_Init(GPIOx, (GPIO_InitTypeDef *)GPIO_Init);

	/* Ensure the pin starts low */
	HAL_GPIO_WritePin(GPIOx, GPIO_Init->Pin, GPIO_PIN_RESET);

}

void Profile_Set(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {

	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);

}

void Profile_Reset(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {

	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);

}

void Profile_Toggle(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {

	HAL_GPIO_TogglePin(GPIOx, GPIO_Pin);

}
