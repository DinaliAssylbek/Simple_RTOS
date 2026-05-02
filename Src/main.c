/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Dinali Assylbek
 * @brief          : Main program body
 ******************************************************************************
 */

#include <stdint.h>

#include "stm32f103xb.h"
#include "tasks.h"
#include "rtos.h"

static void SystemClock_Config(void);

int main(void)
{
	OS_Init();
	OS_AddThread(&Task0, 1);
	OS_AddThread(&Task1, 2);
	OS_AddThread(&Task2, 3);
	OS_Launch(8000); // Switch every 1ms
	return 0;
}

void panic() {

}

static void SystemClock_Config(void) {

	RCC_OscInitTypeDef RCC_OscInstance = {0};
	RCC_OscInstance.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInstance.HSEState = RCC_HSE_ON;
	RCC_OscInstance.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	RCC_OscInstance.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInstance.PLL.Source = RCC_PLLSOURCE_HSE;
	RCC_OscInstance.PLL.PLLMUL = RCC_PLL_MUL9;
	HAL_RCC_OscConfig(&RCC_OscInstance);

	RCC_ClkInitTypeDef RCC_ClkInstance = {0};
	RCC_ClkInstance.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInstance.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInstance.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInstance.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInstance.APB2CLKDivider = RCC_SYSCLK_DIV1;
	HAL_RCC_ClockConfig(&RCC_ClkInstance, FLASH_LATENCY_2);
}
