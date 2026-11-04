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
 
#ifndef IPV4_RUNDOWN
#define IPV4_RUNDOWN

namespace IPV4 {

class Rundown {
public:
  enum : bool {KEEP=false, DELETE=true};
public:
  Rundown()               {}
  Rundown(const Rundown&) {}
public:
  virtual ~Rundown() {}
public:
  virtual bool complete() = 0; // Keep object = false, delete object = true...
};

}

#endif
