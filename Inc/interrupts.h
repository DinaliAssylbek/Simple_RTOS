/*
 * interrupts.h
 *
 *  Created on: Apr 16, 2026
 *      Author: dinaliassylbek
 */

#include <stdint.h>

#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

uint32_t StartCritical(void);
void EndCritical(uint32_t state);

#endif /* INTERRUPTS_H_ */
