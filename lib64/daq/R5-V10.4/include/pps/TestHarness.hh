
/*
**  Package:
**	
**
**  Abstract:
**
** Compare performance  of our own (Steve's) routines with the equivalent routines for 
** invaliding and  flushing cache. My (MEH) observations are as follows:
** - For invalidating cache RTEMS seems to touch each cache line @ ~33 NS/cache line. 
**   Steve's routine seems to touch only those lines which are cached @ ~50 NS/cache line.
** - For flushing cache, both routines seem to touch all cache lines independent of whether 
**   that line is cached or not (a design choice for ARM?). For RTEMS, at ~50 NS/cache line 
**   and for our own ~33 NS cache line. In short, first, for invalidating a buffer Steve's
**   routine is much faster and for flushing Steve's function is roughly 1/3 faster. Second,
**   (using Steve's routine) invalidating is a relatively *cheap* operation independent of
**   # of lines invalidated, but independent of routine flushing is an *expensive* operation 
**   and scales with the buffer size, but Steve does it with fewer instructions.    
**
**  Author:
**      Michael Huffer, SLAC (mehsys@slac.stanford.edu)
**
**  Creation Date:
**	    000 - April 06, 2011
**
**  Revision History:
**	    None.
**
** --
*/
 
#ifndef PPS_TESTHARNESS
#define PPS_TESTHARNESS

#include "pmu/Monitor.hh"
#include "pmu/Profile.hh"

namespace PPS {

class  TestHarness {
public:
  TestHarness();                                                           
public:
 ~TestHarness() {delete[] _buffer;} 
public: 
  void invalidate1(int count);
  void invalidate2(int count);
public:  
  void flush1(int count);
  void flush2(int count);
public:
  void dump(); 
private:
  PMU::Profile  _profile;
  PMU::Monitor  _monitor;
  char*         _buffer;
  };
  
}

#endif


