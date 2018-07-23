/*
 * def.cpp
 *
 *  Created on: Aug 31, 2017
 *      Author: OS1
 */

#include "def.h"

#include <dos.h>
#include "thread.h"
#include "list.h"
#include "kernelev.h"

IVTEntry* IVTEntry::all_entries[256];

IVTEntry::IVTEntry(IVTNo ivtNo, routinePointer new_routine) {
  this->ivtNo = ivtNo;
  this->old_routine = 0;
	#ifndef BCC_BLOCK_IGNORE
	hard_lock;
	this->old_routine = getvect(ivtNo);
	setvect(ivtNo, new_routine);
	hard_unlock;
  #endif
	IVTEntry::all_entries[ivtNo] = this;
	hard_lock;
	events = new List();
	hard_unlock;
}

IVTEntry::~IVTEntry() {
	#ifndef BCC_BLOCK_IGNORE
	hard_lock;
	setvect(ivtNo, old_routine);
	delete events;
	hard_unlock;
	#endif
	IVTEntry::all_entries[ivtNo] = 0;
}

void IVTEntry::signal_entry_event(){
	for (listElement *tek = events->first; tek; tek = tek->nextElement)
		((KernelEv*)tek->element)->signal();
	dispatch();
}
