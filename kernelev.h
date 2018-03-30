/*
** Monolithic multithreading kernel (8086)
** Konstantin Z. Janojlic
*/

#ifndef _kernelev_h_
#define _kernelev_h_

#include "def.h"
#include "pcb.h"
#include "system.h"

class IVTEntry;

class KernelEv {
public:
	KernelEv(IVTNo No);
	~KernelEv();

	void signal();
	void wait();

	unsigned char ivtNo;
	PCB *myPCB;
	int value;
};

#endif
