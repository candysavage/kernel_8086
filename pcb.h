#ifndef _pcb_h_
#define _pcb_h_

#include "thread.h"
#include "list.h"
#include "ksem.h"
#include "idle.h"

class Idle;
class KernelSem;

class PCB {

public:
	unsigned* stack;
	volatile unsigned ss;
	volatile unsigned sp;
	volatile unsigned bp;

	volatile unsigned long timeSlice;
	unsigned long stackSize;
	volatile State state;

	Thread* myThread;

	static unsigned int globalID;
	unsigned int ID;

	Time timeToSleep;

	static List *pcbList;
	static List *sleepingThreads;
	static volatile PCB *Running;
	static Idle *Idle;

	KernelSem *waitToCompleteSem;

	PCB (Thread* _myThread, int _stackSize, int _timeSlice);
	~PCB();
	static unsigned int getRunningId();
	static Thread* getThreadByID(unsigned int id);
	unsigned int getId();
	static void sleep(unsigned long t);
	static void Wrapper();
	void waitToComplete();
	void makeStack(void (*function)(), unsigned long size);
};

#endif
