/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Dinali Assylbek
 * @brief          : Main program body
 ******************************************************************************
 */

#include <stdint.h>
#include "stm32f103xb.h"
#include "rtos.h"

void Profile_Init(void);
void task0(void);
void task1(void);
void task2(void);

int main(void)
{
	OS_Init();
	Profile_Init();
//	OS_AddThreads(&task0, &task1, &task2);
	OS_Launch(8000); // Switch every 1ms
	return 0;
}

void Profile_Init(void) {
	   // Enable GPIOA clock
	    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

	    // Reset Function Mode for PA5, PB2, PC4
	    GPIOA->CRL &= ~(0xF << 20);
	    GPIOB->CRL &= ~(0xF << 8);
	    GPIOC->CRL &= ~(0xF << 16);

		// Configure PA5, PB2, PC4 as output (2 MHz, push-pull)
	    GPIOA->CRL |=  (0x2 << 20);
	    GPIOB->CRL |=  (0x2 << 8);
	    GPIOC->CRL |=  (0x2 << 16);
}

void Profile_Toggle0() {
	GPIOA->ODR ^= (1 << 5);
}

void Profile_Toggle1() {
	GPIOB->ODR ^= (1 << 2);
}

void Profile_Toggle2() {
	GPIOC->ODR ^= (1 << 4);
}

void task0(void) {
	int count0 = 0;
	while (1) {
		count0++;
		Profile_Toggle0();
	}
}

void task1(void) {
	int count1 = 0;
	while (1) {
		count1++;
		Profile_Toggle1();
	}
}

void task2(void) {
	int count2 = 0;
	while (1) {
		count2++;
		Profile_Toggle2();
	}
}

