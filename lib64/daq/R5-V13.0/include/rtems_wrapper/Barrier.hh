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
 
#ifndef RTEMS_BARRIER
#define RTEMS_BARRIER

#include <rtems.h>

namespace RTEMS { 

class Barrier {
public:  
  Barrier();
public: 
  Barrier(const Barrier&) = delete;
public:
 ~Barrier() {}
public:
  void release();
  bool wait(unsigned tmo=0);
private:
  rtems_id _task;
};

}

#endif
