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
 
#ifndef IPV4_HANDLER
#define IPV4_HANDLER

#include "net/ipv4/Address.hh"

namespace IPV4 {

class Handler {
public:
  Handler()               {}
  Handler(const Handler&) {}
public:
  virtual ~Handler() {}
public:
  virtual bool process(const char* payload, int length, const Address&) {return false;};
};

}

#endif
