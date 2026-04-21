#include "scheduler.h"

tcbType tcbs[NUMTHREADS];
tcbType *RunPt;
int32_t Stacks[NUMTHREADS][STACKSIZE];

void Scheduler(void) {

	// Decrement all sleeping threads
	tcbType *p = RunPt;
	do {
		if (p->sleep > 0) {
			p->sleep--;
		}
		p = p->next;
	} while (p != RunPt);

	// Transition to next thread;
	RunPt = RunPt->next;
	while ((RunPt->sleep) || (RunPt->blocked)) {
		RunPt = RunPt->next;
	}
}
