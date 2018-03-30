#include <dos.h>
#include "pcb.h"
#include "system.h"

Idle* PCB::Idle = 0;
List* PCB::pcbList = new List();
volatile PCB* PCB::Running = 0;
unsigned int PCB::globalID = 0;

void PCB::makeStack (void (*function)(), unsigned long size) {
	#ifndef BCC_BLOCK_IGNORE
	hard_lock;
	stack = new unsigned[size];
	hard_unlock;

	stack[size-1] = 0x200;
	stack[size-2] = FP_SEG(function);
	stack[size-3] = FP_OFF(function);

	sp = FP_OFF(stack + size - 12);
	ss = FP_SEG(stack + size - 12);
	bp = FP_OFF(stack + size - 12);
#endif

	state = ready;
}

PCB::PCB(Thread* _myThread, int _stackSize, int _timeSlice) {
	stack = 0;
	ID = ++globalID;
	timeToSleep = 0;
	signalDeblock = 0;
	blockedOn = 0;
	waitToCompleteSem = 0;
	timeSlice = (_timeSlice < 0) ? defaultTimeSlice : _timeSlice;
	if(_stackSize <= 0)
		stackSize = defaultStackSize;
	else
		stackSize = (defaultStackSize <= _stackSize) ? defaultStackSize : _stackSize;
	myThread = _myThread;
	state = created;
	PCB::pcbList->put(this);
#ifndef BCC_BLOCK_IGNORE
	hard_lock;
#endif
	waitToCompleteSem = new KernelSem(0);
#ifndef BCC_BLOCK_IGNORE
	hard_unlock;
#endif
}

PCB::~PCB() {
	#ifndef BCC_BLOCK_IGNORE
	hard_lock;
	if(stack)
		delete[] stack;
	if(waitToCompleteSem)
		delete waitToCompleteSem;
	if(this != 0)
		PCB::pcbList->remove(this);
	hard_unlock;
	#endif
}

unsigned int PCB::getRunningId() {
	return ((PCB*)PCB::Running)->getId();
}

void PCB::Wrapper(){
	PCB::Running->myThread->run();
	while(((PCB*)PCB::Running)->waitToCompleteSem->value < 0)
		((PCB*)PCB::Running)->waitToCompleteSem->signal();
	((PCB*)PCB::Running)->state = finished;
	lock;
	System::shutdown();
}

Thread* PCB::getThreadByID(unsigned int id) {
	PCB* temp = (PCB*)pcbList->findByID(id);
	return temp->myThread;
}

void PCB::waitToComplete() {
	if(state != finished)
		waitToCompleteSem->wait(0);
}

unsigned int PCB::getId() {
	return ID;
}
