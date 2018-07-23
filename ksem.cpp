#include "ksem.h"
#include "schedule.h"
#include "system.h"
#include "pcb.h"

KernelSem::KernelSem(int init) {
	value = init;
	hard_lock;
	blockList = new List();
	hard_unlock;
}

KernelSem::~KernelSem() {
	while(blockList->first)
		signal();
	if(blockList){
		hard_lock;
		delete blockList;
		hard_unlock;
	}
}

void KernelSem::signal() {
	lock;
	++value;
	if(value <= 0) {
		PCB *continued = (PCB*)blockList->getFirst();
		if(continued) {
			continued->state = ready;
			Scheduler::put(continued);
		}
	}
	unlock;
}

int KernelSem::wait(int toBlock) {
	if(toBlock == 0 && value <= 0)
		return -1;
	--value;
	if(value < 0) {
		lock;
		((PCB*)PCB::Running)->state = blocked;
		blockList->put((PCB*)PCB::Running);

		System::block();
		return 1;
	}
	return 0;
}

int KernelSem::val() {
	return value;
}
