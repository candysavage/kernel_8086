#include "ksem.h"
#include "schedule.h"
#include "system.h"
#include "pcb.h"

List* KernelSem::waitTimeList = new List();

KernelSem::KernelSem(int init) {
	value = init;
#ifndef BCC_BLOCK_IGNORE
	hard_lock;
#endif
	blockList = new List();
#ifndef BCC_BLOCK_IGNORE
	hard_unlock;
#endif
}

KernelSem::~KernelSem() {
	while(blockList->first)
		signal();
#ifndef BCC_BLOCK_IGNORE
	hard_lock;
#endif
	if(blockList) delete blockList;
#ifndef BCC_BLOCK_IGNORE
	hard_unlock;
#endif
}

void KernelSem::signal() {
	lock;
	++value;
	if(value < 0) {
		PCB *continued = (PCB*)blockList->getFirst();
		if(continued) {
			KernelSem::waitTimeList->removeSort(continued);
			continued->state = ready;
			continued->blockedOn = 0;
			continued->timeToSleep = 0;
			continued->signalDeblock = 1;
			Scheduler::put(continued);
		}
	}
	unlock;
}

int KernelSem::wait(Time maxTimeToWait) {
	--value;
	if(value < 0) {
		lock;
		((PCB*)PCB::Running)->blockedOn = this;
		((PCB*)PCB::Running)->state = blocked;
		((PCB*)PCB::Running)->timeToSleep = maxTimeToWait;
		blockList->put((PCB*)PCB::Running);
		if(maxTimeToWait > 0)
			KernelSem::waitTimeList->putSort((PCB*)PCB::Running, maxTimeToWait);

		System::block();
		return ((PCB*)PCB::Running)->signalDeblock;
	}

	return 1;
}

int KernelSem::val() {
	return value;
}
