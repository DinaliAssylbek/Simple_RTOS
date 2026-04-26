/*
 * fifo.h
 * Thread-safe, Blocking FIFO Buffer API
 * Description: Interface for a producer-consumer circular buffer.
 *
 * Author: Dinali Assylbek
 */

#ifndef FIFO_H_
#define FIFO_H_

#include <stdint.h>
#include "rtos.h"

#define FIFOSIZE 16		/* Number of elements the FIFO can store before blocking */

/* FIFO Data Structure: Manages circular buffer logic and thread synchronization */
typedef struct
{
    int32_t data[FIFOSIZE];         /* Circular buffer storage */
    int32_t *put_pt;                /* Pointer to next empty slot */
    int32_t *get_pt;                /* Pointer to next available data */
    semaphoreType current_size;     /* Number of elements currently in FIFO */
    semaphoreType room_left;        /* Number of empty slots remaining */
    semaphoreType mutex;            /* Binary semaphore for mutual exclusion */
} fifoType;

void Fifo_Init(fifoType *fifo);

void Fifo_Put(fifoType *fifo, int32_t data);

int32_t Fifo_Get(fifoType *fifo);

#endif /* FIFO_H_ */
