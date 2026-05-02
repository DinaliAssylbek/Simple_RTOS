/*
 * stm32f1xx_it.h
 *
 *  Created on: May 2, 2026
 *      Author: dinaliassylbek
 */

#ifndef STM32F1XX_IT_H_
#define STM32F1XX_IT_H_

/*
 * Processor Exception Handlers
 */
void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);

/*
 * Peripheral Interrupt Handlers
 */
void TIM2_IRQHandler(void);

#endif /* STM32F1XX_IT_H_ */
