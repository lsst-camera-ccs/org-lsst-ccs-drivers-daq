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

#ifndef DSM_LOCATION
#define DSM_LOCATION

#include "dsi/Location.hh"

namespace DSM {

class __attribute__((__packed__)) Location : public DSI::Location {
public: 
  Location();     
public:
//  Location( const Location& clone)           {_location = clone._location;}
//  Location& operator=(const Location& clone) {_location = clone._location; return *this;}
public:
 ~Location() {}
};

}

#endif


