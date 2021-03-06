#ifndef _thread_h_
#define _thread_h_

#include "def.h"

class PCB;
class Idle;

class Thread {
private:
	PCB* myPCB;

protected:
	friend class Idle;
	friend class PCB;
	Thread (StackSize stackSize = defaultStackSize, Time timeSlice =
			defaultTimeSlice);
	virtual void run() {}

public:
	void start();
	void waitToComplete();
	virtual ~Thread();

	static void sleep(Time timeToSleep);

	ID getId();
	static ID getRunningId();
	static Thread* getThreadById(ID id);

};

void dispatch();

#endif
