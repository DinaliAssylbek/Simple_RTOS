/*
 * profile.c
 *
 * Implementation of high-speed profiling functions.
 * Targeted for STM32F103 (NUCLEO-F103RB).
 *
 * Author: Dinali Assylbek
 */

#include "profile.h"

/* Configurations stored in Flash (const) */
const GPIO_InitTypeDef PROF_PA5  = {GPIO_PIN_5,  GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW};
const GPIO_InitTypeDef PROF_PB2  = {GPIO_PIN_2,  GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW};
const GPIO_InitTypeDef PROF_PB13 = {GPIO_PIN_13, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW};
const GPIO_InitTypeDef PROF_PC4  = {GPIO_PIN_4,  GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW};

/* Initializes the clock and GPIO for a specific profiling pin. */
void Profile_Init(GPIO_TypeDef *GPIOx, const GPIO_InitTypeDef *GPIO_Init) {
    /* Enable the Clock for the specific port */
    if      (GPIOx == GPIOA) __HAL_RCC_GPIOA_CLK_ENABLE();
    else if (GPIOx == GPIOB) __HAL_RCC_GPIOB_CLK_ENABLE();
    else if (GPIOx == GPIOC) __HAL_RCC_GPIOC_CLK_ENABLE();

    /* Initialize the pin
     * Note: We cast to (GPIO_InitTypeDef*) to satisfy the HAL's non-const requirement
     */
    HAL_GPIO_Init(GPIOx, (GPIO_InitTypeDef *)GPIO_Init);

    /* Ensure the pin starts LOW */
    HAL_GPIO_WritePin(GPIOx, GPIO_Init->Pin, GPIO_PIN_RESET);
}

/* Sets a profile pin HIGH.*/
void Profile_Set(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
    HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);
}

/* Resets a profile pin LOW. */
void Profile_Reset(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
    HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);
}

/* Toggles the state of a profile pin. */
void Profile_Toggle(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
    HAL_GPIO_TogglePin(GPIOx, GPIO_Pin);
}
