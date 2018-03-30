/*
** Monolithic multithreading kernel (8086)
** Konstantin Z. Janojlic
*/

#include "system.h"
#include "pcb.h"
#include "list.h"
#include "ksem.h"

extern int userMain(int argc, char *argv[]);

int returnValue = 0;

int main(int argc, char *argv[]) {
  PCB::Idle = new Idle();
  PCB::Running = new PCB(0, defaultStackSize, defaultTimeSlice);
  System::timer_routine_override();
  returnValue = userMain(argc, argv);
  System::timer_routine_restore();
  delete PCB::Idle;
  delete PCB::pcbList;
  delete KernelSem::waitTimeList;
  delete PCB::Running;

  return returnValue;
}
