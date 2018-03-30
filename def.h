/* 
** Monolithic multithreading kernel (8086)
** Konstantin Z. Janojlic
*/

#ifndef _def_h_
#define _def_h_

volatile int lock_flag = 0;

#define lock lock_flag = 1;
#define unlock lock_flag = 0;

#define hard_lock asm{pushf; cli}
#define hard_unlock asm popf

typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096;
typedef unsigned int Time;	// time, x 55ms
const Time defaultTimeSlice = 2;	// default = 2 * 55ms
typedef int ID;

typedef enum {created, ready, blocked, running, finished} State;

typedef unsigned char IVTNo;
typedef void interrupt (*routinePointer) (...);

#define PREPAREENTRY(ivtNo, oldRoutine)\
	void interrupt new_routine##ivtNo(...);\
	IVTEntry entry##ivtNo(ivtNo, new_routine##ivtNo);\
	void interrupt new_routine##ivtNo(...){\
		entry##ivtNo.signal_entry_event();\
		if(oldRoutine)\
		entry##ivtNo.old_routine();\
		}

class List;

class IVTEntry {
public:
	IVTEntry(IVTNo ivtNo, routinePointer new_routine);
	~IVTEntry();
	unsigned char ivtNo;
	routinePointer old_routine;

	static IVTEntry* all_entries[];

	List *events;
	void signal_entry_event();
};


#endif
