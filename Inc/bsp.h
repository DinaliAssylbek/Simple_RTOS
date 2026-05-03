/*
 * bsp.h
 * Board Support Package for STM32F103
 * Description: Low-level hardware abstraction including critical section
 * management and system clock configuration.
 *
 * Author: Dinali Assylbek
 */

#ifndef BSP_H_
#define BSP_H_

#include <stdint.h>
#include "stm32f1xx_hal.h"

uint32_t StartCritical(void);

void EndCritical(uint32_t state);

void SystemClock_Config(void);

#endif /* BSP_H_ */
