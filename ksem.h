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
	int wait(int toBlock);
	int val();

	List *blockList;
};

#endif
