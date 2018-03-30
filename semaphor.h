/* 
** Monolithic multithreading kernel (8086)
** Konstantin Z. Janojlic
*/

#ifndef _semaphor_h_
#define _semaphor_h_

#include "def.h"
#include "ksem.h"

class KernelSem;

class Semaphore {
public:
	Semaphore(int init = 1);
	virtual ~Semaphore();

	virtual int wait (Time maxTimeToWait);
	virtual void signal();

	int val() const;		// Returns the current value of the semaphore

private:
	KernelSem* myImpl;
};

#endif
