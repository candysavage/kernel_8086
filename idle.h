/*
** Monolithic multithreading kernel (8086)
** Konstantin Z. Janojlic
*/

#ifndef _IDLE_H_
#define _IDLE_H_

#include "thread.h"
#include "pcb.h"

class PCB;
class Thread;

class Idle : public Thread {
public:
	Idle();
	virtual void run();
	PCB *pcb;
};

#endif
