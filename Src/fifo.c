/*
 * fifo.c
 * Thread-safe, Blocking FIFO Buffer
 * Description: Implements a producer-consumer circular buffer using
 * three-semaphore synchronization logic.
 *
 * Author: Dinali Assylbek
 */

//==================================================================================================
// INCLUDES
//==================================================================================================

#include "fifo.h"
#include "rtos.h"

//==================================================================================================
// GLOBAL FUNCTIONS
//==================================================================================================

void Fifo_Init(fifoType *fifo) {

	fifo->get_pt = &fifo->data[0];
	fifo->put_pt = &fifo->data[0];

	OS_InitSemaphore(&(fifo->current_size), 0);
	OS_InitSemaphore(&(fifo->room_left), FIFOSIZE);
	OS_InitSemaphore(&(fifo->mutex), 1);

}

void Fifo_Put(fifoType *fifo, int32_t data) {

	OS_Wait(&(fifo->room_left));	/* Block if FIFO is full */
	OS_Wait(&(fifo->mutex));		/* Enter critical section */

	*(fifo->put_pt) = data;
	fifo->put_pt++;

	/* Wrap around pointer if at end of buffer */
	if (fifo->put_pt == &(fifo->data[FIFOSIZE])) {
		fifo->put_pt = &(fifo->data[0]);
	}

	OS_Signal(&(fifo->mutex));			/* Leave critical section */
	OS_Signal(&(fifo->current_size));	/* Notify that data is available */

}

int32_t Fifo_Get(fifoType *fifo) {

	OS_Wait(&(fifo->current_size));		/* Block if FIFO is empty */
	OS_Wait(&(fifo->mutex));			/* Enter critical section */

	int32_t data = *(fifo->get_pt);
	fifo->get_pt++;

	/* Wrap around pointer if at end of buffer */
	if (fifo->get_pt == &(fifo->data[FIFOSIZE])) {
		fifo->get_pt = &(fifo->data[0]);
	}

	OS_Signal(&(fifo->mutex));		/* Leave critical section */
	OS_Signal(&(fifo->room_left));	/* Notify that room is available */

	return data;
}
