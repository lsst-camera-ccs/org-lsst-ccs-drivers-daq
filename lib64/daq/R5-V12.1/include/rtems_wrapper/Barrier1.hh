/*
** ++
**  Package:
**	
**
**  Abstract:
**      
**
**  Author:
**      Michael Huffer, SLAC (mehsys@slac.stanford.edu)
**
**  Creation Date:
**	000 - , 2007
**
**  Revision History:
**	None.
**
** --
*/
 
#ifndef RTEMS_BARRIER1
#define RTEMS_BARRIER1

#include <rtems.h>

namespace RTEMS { 

class Barrier1 {
public:  
  Barrier1(rtems_event_set events=RTEMS_EVENT_31);
public: 
  Barrier1()                = delete;
  Barrier1(const Barrier1&) = delete;
public:
 ~Barrier1() {}
public:
  void release(rtems_event_set event=RTEMS_EVENT_31);
  bool wait(unsigned tmo=0);
private:
  rtems_event_set _events;
  rtems_id        _task;
};

}

#endif
