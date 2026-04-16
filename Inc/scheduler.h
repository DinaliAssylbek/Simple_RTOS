/*
 * scheduler.h
 *
 *  Created on: Apr 15, 2026
 *      Author: dinaliassylbek
 */

#include <stdint.h>

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#define STACKSIZE 100
#define NUMTHREADS 3

struct tcb {
	int32_t *sp; 		// Pointer to Stack (Valid for non active threads)
	struct tcb *next;	// Pointer to next tcb node
};

typedef struct tcb tcbType;
extern tcbType tcbs[NUMTHREADS];
extern tcbType *RunPt;
extern int32_t Stacks[NUMTHREADS][STACKSIZE];


#endif /* SCHEDULER_H_ */
