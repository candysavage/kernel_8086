#include "thread.h"
#include "pcb.h"
#include "system.h"
#include "SCHEDULE.h"


Thread::Thread (StackSize stackSize, Time timeSlice) {
#ifndef BCC_BLOCK_IGNORE
	hard_lock;
#endif
	myPCB = new PCB(this, stackSize, timeSlice);
#ifndef BCC_BLOCK_IGNORE
	hard_unlock;
#endif
}

void Thread::start() {
	myPCB->makeStack(PCB::Wrapper, myPCB->stackSize);
	if(myPCB != PCB::Idle->pcb)
		Scheduler::put(myPCB);
}

Thread::~Thread() {
	waitToComplete();
#ifndef BCC_BLOCK_IGNORE
	hard_lock;
	delete myPCB;
	hard_unlock;
#endif
}

void Thread::waitToComplete() {
	myPCB->waitToComplete();
}

ID Thread::getRunningId() {
	return PCB::getRunningId();
}

Thread* Thread::getThreadById(ID id) {
	return PCB::getThreadByID(id);
}

void dispatch() {
#ifndef BCC_BLOCK_IGNORE
	hard_lock;
#endif
	System::contextSwitch = 1;
	unlock;
	asm int 8h;
#ifndef BCC_BLOCK_IGNORE
	hard_unlock;
#endif
}
