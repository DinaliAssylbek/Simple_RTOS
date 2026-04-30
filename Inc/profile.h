/*
 * profile.h
 *
 *  Created on: Apr 29, 2026
 *      Author: dinaliassylbek
 */

#ifndef PROFILE_H_
#define PROFILE_H_

#include "stm32f103xb.h"

// We add 'letter' as an argument so we can build the RCC name correctly
#define DEFINE_PROFILE_PIN(name, letter, port, pin, bit_offset, reg) \
    void Profile_Init##name(void) { \
        RCC->APB2ENR |= RCC_APB2ENR_IOP##letter##EN; \
        port->reg &= ~(0xF << bit_offset); \
        port->reg |=  (0x2 << bit_offset); \
    } \
    void Profile_Toggle##name(void) { port->ODR ^= (1 << pin); } \
    void Profile_Reset##name(void)  { port->BRR = (1 << pin); }

#define DECLARE_PROFILE_PIN(name) \
    void Profile_Init##name(void); \
    void Profile_Toggle##name(void); \
    void Profile_Reset##name(void);

DECLARE_PROFILE_PIN(PA5)
DECLARE_PROFILE_PIN(PB2)
DECLARE_PROFILE_PIN(PB13)
DECLARE_PROFILE_PIN(PC4)

#endif
