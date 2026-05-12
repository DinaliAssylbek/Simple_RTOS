/*
 * profile.h
 *
 * Hardware profiling driver for STM32F103.
 *
 * Author: Dinali Assylbek
 */

#ifndef PROFILE_H_
#define PROFILE_H_

#include "stm32f103xb.h"
#include "stm32f1xx_hal.h"

/*
 * Profile Pins
 */
extern const GPIO_InitTypeDef PROF_PA5;
extern const GPIO_InitTypeDef PROF_PB2;
extern const GPIO_InitTypeDef PROF_PB13;
extern const GPIO_InitTypeDef PROF_PC4;

/* API Functions */
void Profile_Init(GPIO_TypeDef *GPIOx, const GPIO_InitTypeDef *GPIO_Init);
void Profile_Set(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void Profile_Reset(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void Profile_Toggle(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

#endif /* PROFILE_H_ */
