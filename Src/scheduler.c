#include "scheduler.h"

tcbType tcbs[NUMTHREADS];
tcbType *RunPt;
tcbType *SleepListHead;
tcbType *ReadyListHead;
int32_t Stacks[NUMTHREADS][STACKSIZE];

void Scheduler(void) {

	// Decrement all sleeping threads
	if (SleepListHead != NULL) {

		SleepListHead->sleep--;

		while (SleepListHead != NULL && SleepListHead->sleep == 0) {
			tcbType *p = SleepListHead;
			SleepListHead = SleepListHead->next;

			p->next = ReadyListHead;
			p->prev = ReadyListHead->prev;

			p->prev->next = p;
			p->next->prev = p;

			ReadyListHead = p;
		}

	}

	// Transition to next thread;
	RunPt = ReadyListHead;
	ReadyListHead = RunPt->next;
}
