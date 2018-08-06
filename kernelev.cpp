#include "kernelev.h"
#include "SCHEDULE.H"
#include "thread.h"

KernelEv::KernelEv(IVTNo No) {
  myPCB = (PCB*)PCB::Running;
  ivtNo = No;
  value = 0;
  if(IVTEntry::all_entries[ivtNo] != 0)
    IVTEntry::all_entries[ivtNo]->events->put(this);
}

KernelEv::~KernelEv() {
  if(IVTEntry::all_entries[ivtNo] != 0)
    IVTEntry::all_entries[ivtNo]->events->remove(this);
}

void KernelEv::signal() {
  lock;
  if((++value) <= 0) {
    myPCB->state = ready;
    Scheduler::put(myPCB);
  }
  unlock;
}

void KernelEv::wait() {
  if(myPCB != (PCB*)PCB::Running) return;
  if((--value) < 0) {
    lock;
    myPCB->state = blocked;
    dispatch();
  }
}
