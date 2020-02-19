
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
 
#ifndef XDS_LINKEDEVENT
#define XDS_LINKEDEVENT

#include "linked/Element.hh"
#include "xds/Id.hh"
#include "xds/Event.hh"

namespace XDS {

class LinkedEvent : public Linked::Element {
public:
  LinkedEvent() = delete;
public:
  LinkedEvent(const Id& id,  uint64_t offset, uint64_t length) : content(id, offset, length) {}
  LinkedEvent(const LinkedEvent& clone)                        : content(clone.content)      {}
public:
 ~LinkedEvent() {}
public:  
  LinkedEvent& operator=(const LinkedEvent& clone) {content = clone.content; return *this;}
public:
  LinkedEvent* flink() {return (LinkedEvent*)Linked::Element::flink();}
  LinkedEvent* blink() {return (LinkedEvent*)Linked::Element::blink();}
public:
  Event content;
  };

}

#endif

