/*
** Monolithic multithreading kernel (8086)
** Konstantin Z. Janojlic
*/

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


  static void timer_routine_override();
  static void timer_routine_restore();

  static void interrupt timer();

  static void block();
  static void shutdown();


};

#endif
