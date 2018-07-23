#include "thread.h"
#include "pcb.h"
#include "system.h"
#include "SCHEDULE.h"


Thread::Thread (StackSize stackSize, Time timeSlice) {
	hard_lock;
	myPCB = new PCB(this, stackSize, timeSlice);
	hard_unlock;
}

void Thread::start() {
	myPCB->makeStack(PCB::Wrapper, myPCB->stackSize);
	if(myPCB != PCB::Idle->pcb)
		Scheduler::put(myPCB);
}

Thread::~Thread() {
	waitToComplete();
	hard_lock;
	delete myPCB;
	hard_unlock;
}

void Thread::waitToComplete() {
	myPCB->waitToComplete();
}

void Thread::sleep(Time timeToSleep) {
	PCB::sleep((unsigned long) timeToSleep);
}
ID Thread::getRunningId() {
	return PCB::getRunningId();
}

Thread* Thread::getThreadById(ID id) {
	return PCB::getThreadByID(id);
}

void dispatch() {
	hard_lock;
	System::contextSwitch = 1;
	unlock;
	asm int 8h;
	hard_unlock;
}
