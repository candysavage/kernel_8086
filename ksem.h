/*
** Monolithic multithreading kernel (8086)
** Konstantin Z. Janojlic
*/

#ifndef _ksem_h_
#define _ksem_h_

#include "def.h"
#include "list.h"

class KernelSem {
public:
	int value;

	KernelSem(int init);
	~KernelSem();

	void signal();
	int wait(Time maxTimeToWait);
	int val();

	static List *waitTimeList;
	List *blockList;
};

#endif
