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
 
#ifndef PMU_TIMER
#define PMU_TIMER

#include <stdint.h>

namespace PMU {

class Timer {
public:
  static unsigned nanoseconds(unsigned tics)  {return (tics * 2) + (tics /2);}
public:
  Timer();
  Timer(const Timer& clone) : _timer(clone._timer), _start(clone._start) {}
public:
 ~Timer() {}
public:
  void     start() {_start = *_timer;}
  uint32_t stop()  {uint32_t current = *_timer; return current - _start;}
public:
  void dump(unsigned elapsed);
private:   
  uint32_t* _timer;
  uint32_t  _start;
};

}

#endif
