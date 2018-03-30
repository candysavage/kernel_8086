#include "idle.h"
#include "def.h"

Idle::Idle() {
	Thread(defaultStackSize, defaultTimeSlice);
	this->pcb = (PCB*)this->myPCB;
	start();
}

void Idle::run() {
a:
  #ifndef BCC_BLOCK_IGNORE
  asm{
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
  }
  #endif
  goto a;
}
