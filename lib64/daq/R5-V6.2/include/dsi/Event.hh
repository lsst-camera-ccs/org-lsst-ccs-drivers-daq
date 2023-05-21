
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
 
#ifndef DSI_EVENT
#define DSI_EVENT

#include <stdint.h>

#include "dsi/Set.hh"

namespace DSI {

class __attribute__((__packed__)) Event : public Set {
public:
  Event()                   : Set()       {}      
  Event(uint8_t accept)     : Set(accept) {}   
  Event(Set&    accept)     : Set(accept) {}   
  Event(const Event& clone) : Set(clone)  {}
public:
 ~Event() {} 
public:  
  Event& operator=(const Event& clone) {*this = clone; return *this;}
  };

static_assert(!(sizeof(Event) & 7), "Class DSI::Event is not an even number of quadwords");

}

#endif

