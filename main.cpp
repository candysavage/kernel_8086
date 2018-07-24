#include "system.h"
#include "pcb.h"
#include "list.h"
#include "ksem.h"

extern int userMain(int argc, char *argv[]);

int returnValue = 0;

int main(int argc, char *argv[]) {
  PCB::Idle = new Idle();
  PCB::Running = new PCB(0, defaultStackSize, defaultTimeSlice);
  System::timerRoutineOverride();
  returnValue = userMain(argc, argv);
  System::timerRoutineRestore();
  delete PCB::Idle;
  delete PCB::pcbList;
  delete KernelSem::waitTimeList;
  delete PCB::Running;

  return returnValue;
}
