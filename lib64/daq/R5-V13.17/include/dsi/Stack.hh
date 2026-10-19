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
 
#ifndef DSI_STACK
#define DSI_STACK

#include "dsi/LocationSet.hh"

namespace DSI {

class Stack { 
public:
  Stack(const LocationSet& partition, const LocationSet& filter) : _stack(partition), _filter(filter), _partition(partition) {~_filter;}
public:
  Stack()             = delete;
  Stack(const Stack&) = delete;  
public:
 ~Stack() {}
public:
  const LocationSet& servers()   const {return _stack;} 
  const LocationSet& partition() const {return _partition;} 
public:  
  bool accept(const LocationSet& servers, LocationSet& invalid) const  {invalid  = servers; invalid &= _filter; return !invalid;}
public:
  void insert(const LocationSet& servers) {_stack |= servers;}
  void empty()                            {LocationSet empty; _stack = empty;}
private:
  LocationSet _stack;
  LocationSet _filter;
  LocationSet _partition;  
};

}

#endif
