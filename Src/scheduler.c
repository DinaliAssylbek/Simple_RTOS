#include "scheduler.h"

tcbType tcbs[NUMTHREADS];
tcbType *RunPt;
int32_t Stacks[NUMTHREADS][STACKSIZE];

void Scheduler(void) {
	RunPt = RunPt->next;
	while (RunPt->blocked) {
		RunPt = RunPt->next;
	}
}
