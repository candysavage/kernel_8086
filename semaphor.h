#ifndef _semaphor_h_
#define _semaphor_h_

#include "def.h"
#include "ksem.h"

class KernelSem;

class Semaphore {
public:
	Semaphore(int init = 1);
	virtual ~Semaphore();

	virtual int wait (int toBlock);
	virtual void signal();

	int val() const;

private:
	KernelSem* myImpl;
};

#endif
