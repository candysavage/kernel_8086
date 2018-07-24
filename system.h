#ifndef _system_h_
#define _system_h_

#include "def.h"

class System {
public:

	volatile static int contextSwitch;
	volatile static int globalFlag;       /*
                                             * 1 - block Running thread
                                             * 2 - shutdown Running thread
                                             */


  static void timerRoutineOverride();
  static void timerRoutineRestore();

  static void interrupt timer();

  static void block();
  static void shutdown();


};

#endif
