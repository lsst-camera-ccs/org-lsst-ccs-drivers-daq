
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
 
#ifndef DSID_MAP
#define DSID_MAP

#include "dsi/Id.hh"
#include "dsi/Frame.hh"
#include "dsid/Functions.hh"

namespace DSID {

class Map : public DSI::Frame {
public:
  Map()                 = delete;
  Map(const Map& clone) = delete;
public:
  Map(unsigned id) : Frame(MAP) {set((int32_t)id);}  
public:
 ~Map() {} 
public:
  operator bool() const {return status() != DSI::Id::MAX;} 
 };

}

#endif

