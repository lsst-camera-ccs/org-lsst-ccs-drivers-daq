
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
 
#ifndef DSI_SHUTDOWN
#define DSI_SHUTDOWN

#include "dsi/Frame.hh"

namespace DSI {

class  Shutdown : public Frame {
public:
  Shutdown() : Frame(Frame::SHUTDOWN) {} 
public:  
  Shutdown(const Shutdown&) = delete; 
public:
 ~Shutdown() {}        
 };

}

#endif

