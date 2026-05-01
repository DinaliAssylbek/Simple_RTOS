/*
 * profile.c
 *
 * Implementation of high-speed profiling functions.
 * Targeted for STM32F103 (NUCLEO-F103RB).
 *
 * Author: Dinali Assylbek
 */

#include "profile.h"

/* Parameters: {Port, Pin, Bit-Offset, RCC-Bit, Config-Register} */
const ProfilePinType PROF_PA5 = {GPIOA, 5, 20, 2, &GPIOA->CRL};
const ProfilePinType PROF_PB2 = {GPIOB, 2, 8,  3, &GPIOB->CRL};
const ProfilePinType PROF_PB13 = {GPIOB, 13, 20, 3, &GPIOB->CRH};
const ProfilePinType PROF_PC4 = {GPIOC, 4, 16, 4, &GPIOC->CRL};

void Profile_Init(const ProfilePinType *p) {
    RCC->APB2ENR |= (1 << p->rcc_pos);			/* Enable GPIO Bank Clock in RCC */
    *(p->config_reg) &= ~(0xF << p->offset);	/* Clear 4-bit configuration field for this pin */
    *(p->config_reg) |=  (0x2 << p->offset);	/* Set to General Purpose Output Push-Pull (Max speed 2MHz) */
}

void Profile_Toggle(const ProfilePinType *p) {
    p->port->ODR ^= (1 << p->pin);				/* Flip the bit in Output Data Register using XOR */
}

void Profile_Reset(const ProfilePinType *p) {
    p->port->BRR = (1 << p->pin);				/* Atomic clear using Bit Reset Register */
}
