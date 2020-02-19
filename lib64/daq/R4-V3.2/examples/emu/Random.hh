
/*
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
**	    000 - April 06, 2011
**
**  Revision History:
**	    None.
**
** --
*/
 
#ifndef EMU_RANDOM
#define EMU_RANDOM

#include "ims/science/Data.hh"
#include "ims/guiding/Data.hh"
#include "ims/wavefront/Data.hh"

namespace EMU {
 
class Random  {
public:
 enum : uint64_t {SAMPLES = 7382};
public:                     
  Random();
public:
  Random(const Random&) = delete; 
public:
 ~Random();
public:
  IMS::Science::Data&   science()   {return _science;} 
  IMS::Guiding::Data&   guiding()   {return _guiding;} 
  IMS::Wavefront::Data& wavefront() {return _wavefront;}     
private:
  char*                _buffer;     
  IMS::Science::Data   _science;
  IMS::Guiding::Data   _guiding; 
  IMS::Wavefront::Data _wavefront;  
};

}

#endif

