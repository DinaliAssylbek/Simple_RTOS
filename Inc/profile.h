/*
 * profile.h
 *
 * Hardware profiling driver for STM32F103.
 * Uses direct register access for high-speed task timing.
 *
 * Author: Dinali Assylbek
 */

#ifndef PROFILE_H_
#define PROFILE_H_

#include "stm32f103xb.h"
#include <stdint.h>

/* Hardware Descriptor */
typedef struct {
    GPIO_TypeDef *port;			/* GPIO Port (GPIOA, GPIOB, etc.) */
    uint8_t       pin;			/* Pin number (0-15) */
    uint8_t       offset;		/* Bit offset in configuration register */
    uint8_t       rcc_pos;		/* Clock enable bit position in APB2ENR */
    __IO uint32_t *config_reg;	/* Pointer to CRL or CRH register */
} ProfilePinType;

/* Available Profile Pins */
extern const ProfilePinType PROF_PA5;
extern const ProfilePinType PROF_PB2;
extern const ProfilePinType PROF_PB13;
extern const ProfilePinType PROF_PC4;

/* API Functions */
void Profile_Init(const ProfilePinType *p);
void Profile_Toggle(const ProfilePinType *p);
void Profile_Reset(const ProfilePinType *p);

#endif

