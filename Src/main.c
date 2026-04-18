/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Dinali Assylbek
 * @brief          : Main program body
 ******************************************************************************
 */

#include <stdint.h>
#include "stm32f103xb.h"

int main(void)
{
    // Enable GPIOC clock
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    // Configure PC13 as output (push-pull, 2 MHz)
    GPIOA->CRL &= ~(0xF << 20);
    GPIOA->CRL |=  (0x2 << 20);

    while (1) {
    	GPIOA->ODR ^= (1 << 5); // FORCE HIGH
    	for (int i = 0; i < 25000; i++);
    }
}

void task0(void) {
	int count = 0;
	while (true) {
		count++;
	}
}

void task1(void) {
	int count = 0;
	while (true) {
		count++;
	}
}

void task2(void) {
	int count = 0;
	while (true) {
		count++;
	}
}

