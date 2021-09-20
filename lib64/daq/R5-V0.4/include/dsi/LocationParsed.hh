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
**	000 - , 2007
**
**  Revision History:
**	None.
**
** --
*/
 
#ifndef DSI_LOCATIONPARSED
#define DSI_LOCATIONPARSED

#include "dsi/Location.hh"

namespace DSI {

class LocationParsed  : public Location {
public: 
  LocationParsed()                            : Location(UNDEFINED), _bays(0),           _rces(0)           {}
  LocationParsed(const LocationParsed& clone) : Location(clone),     _bays(clone._bays), _rces(clone._rces) {}
public:
  LocationParsed(const char* location);                   
public:  
  LocationParsed& operator=(const LocationParsed& clone) {Location::operator=(clone); _bays = clone._bays; _rces = clone._rces; return *this;}
public:
 ~LocationParsed() {} 
public:
  unsigned rces() const {return _rces;}      
  unsigned bays() const {return _bays;} 
private:
 unsigned _bays;
 unsigned _rces;
};

}

#endif
