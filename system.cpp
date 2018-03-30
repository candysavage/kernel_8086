#include "system.h"
#include <dos.h>
#include "thread.h"
#include "pcb.h"
#include "ksem.h"
#include "SCHEDULE.H"

volatile unsigned ss;
volatile unsigned sp;
volatile unsigned bp;

volatile unsigned tss;
volatile unsigned tsp;
volatile unsigned tbp;

volatile unsigned int timeRunning = 0;

volatile int System::contextSwitch = 0;
volatile int System::globalFlag = 0;

extern void tick();


void interrupt System::timer () {	
	if(System::contextSwitch == 1 || 
		(PCB::Running->timeSlice > 0 && timeRunning >= PCB::Running->timeSlice)) {
		if(lock_flag == 0) {
			#ifndef BCC_BLOCK_IGNORE
			asm {
				mov tsp, sp
				mov tss, ss
				mov tbp, bp
			}
			#endif
			PCB::Running->ss = tss;
			PCB::Running->sp = tsp;
			PCB::Running->bp = tbp;

			switch(System::globalFlag)
			{
				case 1:													// PCB::Running blocked
				PCB::Running = Scheduler::get(); break;

				case 2:													// PCB::Running finished
				PCB::Running->state = finished;
				PCB::Running = Scheduler::get(); break;

				default:												// Regular context switch (globalFlag == 0)
				if(PCB::Running != PCB::Idle->pcb && PCB::Running->state != blocked) {
					PCB::Running->state = ready;
					Scheduler::put((PCB*)PCB::Running);
				}
				PCB::Running = Scheduler::get();
				break;
			}

			if(PCB::Running == 0)
				PCB::Running = PCB::Idle->pcb;

			PCB::Running->state = running;
			tss = PCB::Running->ss;
			tsp = PCB::Running->sp;
			tbp = PCB::Running->bp;

			#ifndef BCC_BLOCK_IGNORE
			asm {
				mov ss, tss;
				mov sp, tsp;
				mov bp, tbp;
			}
			#endif

			timeRunning = 0;
			System::contextSwitch = 0;
			System::globalFlag = 0;
		}	else {
			System::contextSwitch = 1;
		}
	}

	if(System::contextSwitch == 0 || lock_flag == 1) {
		tick();

		++timeRunning;

		if(KernelSem::waitTimeList != 0) {
			if(KernelSem::waitTimeList->first != 0) {
				--KernelSem::waitTimeList->first->timeToSleep;
				while(KernelSem::waitTimeList->first->timeToSleep == 0) {
					PCB* temp = (PCB*)KernelSem::waitTimeList->first->element;
					KernelSem::waitTimeList->removeSort(temp);
					temp->blockedOn->blockList->remove(temp);
					++temp->blockedOn->value;

					temp->state = ready;
					temp->signalDeblock = 0;
					if(temp != PCB::Idle->pcb)
						Scheduler::put((PCB*)temp);
				}
			}
		}

		#ifndef BCC_BLOCK_IGNORE
		asm int 62h;
		#endif

	}

}



void System::timer_routine_override () {
#ifndef BCC_BLOCK_IGNORE
	hard_lock;
	routinePointer old = getvect(0x8);
	setvect(0x62, old);
	setvect(0x8, (routinePointer)System::timer);
	hard_unlock;
#endif
}

void System::timer_routine_restore () {
#ifndef BCC_BLOCK_IGNORE
	hard_lock;
	routinePointer old = getvect(0x62);
	setvect(0x8,old);
  	hard_unlock;
#endif;
}

void System::block() {
	lock;
	System::globalFlag = 1;
	dispatch();
}

void System::shutdown() {
	lock;
	System::globalFlag = 2;
	dispatch();
}
