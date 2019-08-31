
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
 
#ifndef DSID_FIND
#define DSID_FIND

#include "dsi/Frame.hh"
#include "dsid/Functions.hh"

namespace DSID {

class Find : public DSI::Frame {
public:
  Find(const Find& clone) = delete;
public:
  Find() : Frame(FIND) {}  
public:
 ~Find() {} 
 };

}

#endif

