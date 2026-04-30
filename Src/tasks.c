/*
 * tasks.c
 *
 *  Created on: Apr 29, 2026
 *      Author: dinaliassylbek
 */

void Profile_InitPA5(void) {
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	GPIOA->CRL &= ~(0xF << 20);
	GPIOA->CRL |=  (0x2 << 20);
}

void Profile_InitPB2(void) {
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	GPIOB->CRL &= ~(0xF << 8);
	GPIOB->CRL |=  (0x2 << 8);
}

void Profile_InitPB13(void) {
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	GPIOB->CRH &= ~(0xF << 20);
	GPIOB->CRH |= (0x2 << 20);
}

void Profile_InitPC4(void) {
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	GPIOC->CRL &= ~(0xF << 16);
	GPIOC->CRL |=  (0x2 << 16);
}

void Profile_TogglePA5(void) {
	GPIOA->ODR ^= (1 << 5);
}

void Profile_TogglePB2(void) {
	GPIOB->ODR ^= (1 << 2);
}

void Profile_TogglePB13(void) {
	GPIOB->ODR ^= (1 << 13);
}

void Profile_TogglePC4(void) {
	GPIOC->ODR ^= (1 << 4);
}

void Profile_ResetPA5(void) {
	GPIOA->ODR &= (1 << 5);
}

void Profile_ResetPB2(void) {
	GPIOB->ODR &= (1 << 2);
}

void Profile_ResetPB13(void) {
	GPIOB->ODR &= (1 << 13);
}

void Profile_ResetPC4(void) {
	GPIOC->ODR &= (1 << 4);
}

void task0(void) {
	Profile_InitPA5();
	uint32_t count = 0;
	while (1) {
		Profile_TogglePA5();
		count++;
		if (count == 100) {
			OS_CreateThread(&task3, 1);
		}
		if (count == 200) {
			Profile_ResetPA5();
			OS_KillThread();
		}
	}
}

void task1(void) {
	Profile_InitPB2();
	while (1) {
		for (int i = 0; i < 12; i++) {
			Profile_TogglePB2();
			// Delay
		}

		OS_Suspend();
	}
}

void task2(void) {
	Profile_InitPC4();
	uint32_t count = 0;
	while (1) {
		Profile_TogglePC4();
		count++;
		if (count % 35 == 0) {
			OS_Sleep(4500);
		} else {
			// delay
		}
	}
}

void task3(void) {
	Profile_InitPB13();
	while (1) {
		Profile_TogglePB13();
		// delay
	}
}

