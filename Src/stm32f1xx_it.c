/*
 * stm32f1xx_it.c
 * Interrupt Service Routines
 * Description: Redirects hardware interrupts to the HAL and OS Scheduler.
 *
 * Author: Dinali Assylbek
 */

//==================================================================================================
// INCLUDES
//==================================================================================================

#include "stm32f1xx_it.h"
#include "stm32f1xx_hal.h"
#include "rtos.h"

//==================================================================================================
// CORTEX-M3 PROCESSOR EXCEPTIONS
//==================================================================================================

void NMI_Handler(void) {}

void HardFault_Handler(void) {
	while (1) {} /* Add a breakpoint here during debugging */
}

void MemManage_Handler(void) {
	while (1) {}
}

void BusFault_Handler(void) {
	while (1) {}
}

void UsageFault_Handler(void) {
	while (1) {}
}

void SVC_Handler(void) {}

void DebugMon_Handler(void) {}

void PendSV_Handler(void) {}

void SysTick_Handler(void) {
	HAL_IncTick(); /* Essential for HAL_Delay() and timeout logic */
}

//==================================================================================================
// STM32F1xx PERIPHERAL INTERRUPT HANDLERS
//==================================================================================================

/*
 * TIM2 global interrupt
 * This is the heartbeat of your RTOS scheduler.
 */
void TIM2_IRQHandler(void) {
	OS_Scheduler();
}
