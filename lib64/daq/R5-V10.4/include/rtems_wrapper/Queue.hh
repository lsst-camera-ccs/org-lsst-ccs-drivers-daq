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
**	000 - January 09, 2007
**
**  Revision History:
**	None.
**
** --
*/
 
#ifndef RTEMS_QUEUE
#define RTEMS_QUEUE

#include <rtems.h>

namespace RTEMS  {

class Queue {
public:
  Queue(unsigned depth);
public:
  Queue()             = delete;
  Queue(const Queue&) = delete;
public:
 ~Queue();
public:  
  bool  insert(void* message);                  // Insert @ tail...
  void* remove(unsigned tmo=RTEMS_NO_TIMEOUT);  // Remove from tail (RTEMS_WAIT)...
  void* probe();                                // Remove from tail (RTEMS_NO_WAIT)...
public:
  unsigned pending() const;
  unsigned flush();
private:
  rtems_id _queue; 
};

}

#endif
