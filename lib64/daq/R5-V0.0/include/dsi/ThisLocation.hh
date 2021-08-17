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

#ifndef DSI_THISLOCATION
#define DSI_THISLOCATION

#include "dsi/Location.hh"

namespace DSI {

class __attribute__((__packed__)) ThisLocation : public Location {
public: 
  ThisLocation();     
public:
//  ThisLocation(const ThisLocation& clone)           {_location = clone._location;}
//  ThisLocation& operator=(const ThisLocation& clone) {_location = clone._location; return *this;}
public:
 ~ThisLocation() {}
};

}

#endif


