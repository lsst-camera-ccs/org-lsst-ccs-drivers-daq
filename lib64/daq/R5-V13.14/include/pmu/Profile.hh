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
 
#ifndef PMU_PROFILE
#define PMU_PROFILE

#include <stdint.h>

#include "pmu/Timer.hh"

namespace PMU {

class Profile {
public:
  enum {MAX=64};
public:
  Profile() : _timer(), _next(0) {}
public:  
  Profile(const Profile&) = delete;
public:
 ~Profile() {}
public:
  void start() {_timer.start();}
  void stop()  {unsigned next = _next; _tics[next++ & (MAX-1)] = _timer.stop(); _next = next;}
public:
  void dump();
private:   
  Timer     _timer;
  unsigned  _next;
  unsigned  _tics[MAX];
};

}

#endif
