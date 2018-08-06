#include "def.h"

#include <dos.h>
#include "thread.h"
#include "list.h"
#include "kernelev.h"

IVTEntry* IVTEntry::all_entries[256];

IVTEntry::IVTEntry(IVTNo ivtNo, routinePointer new_routine) {
  this->ivtNo = ivtNo;
  this->old_routine = 0;
  hard_lock;
  #ifndef BCC_BLOCK_IGNORE
	this->old_routine = getvect(ivtNo);
	setvect(ivtNo, new_routine);
  #endif
	hard_unlock;
  IVTEntry::all_entries[ivtNo] = this;
	hard_lock;
	events = new List();
	hard_unlock;
}

IVTEntry::~IVTEntry() {
	hard_lock;
  #ifndef BCC_BLOCK_IGNORE
	setvect(ivtNo, old_routine);
  #endif
  delete events;
	hard_unlock;
	IVTEntry::all_entries[ivtNo] = 0;
}

void IVTEntry::signal_entry_event(){
	for (listElement *tek = events->first; tek; tek = tek->nextElement)
		((KernelEv*)tek->element)->signal();
	dispatch();
}
