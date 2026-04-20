/*
 * fifo.h
 *
 *  Created on: Apr 19, 2026
 *      Author: dinaliassylbek
 */

#ifndef FIFO_H_
#define FIFO_H_

#include <stdint.h>

#define FIFOSIZE 16

void OS_Fifo_Init(void);
void OS_Fifo_Put(int32_t data);
int32_t OS_Fifo_Get(void);

#endif /* FIFO_H_ */
