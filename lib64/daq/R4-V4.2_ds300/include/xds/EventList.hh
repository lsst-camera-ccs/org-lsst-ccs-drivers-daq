
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
 
#ifndef XDS_EVENTLIST
#define XDS_EVENTLIST

#include "linked/List.hh"
#include "xds/LinkedEvent.hh"

namespace XDS {

class EventList: public Linked::List {
public:
  EventList() : Linked::List() {} 
public:
 ~EventList() {}
public:
  LinkedEvent* head() {return (LinkedEvent*)Linked::List::head();} 
  LinkedEvent* tail() {return (LinkedEvent*)Linked::List::tail();}  
  LinkedEvent* last() {return (LinkedEvent*)Linked::List::last();}
public:  
  LinkedEvent* remove() {return (LinkedEvent*)Linked::List::remove();} 
};

}

#endif

