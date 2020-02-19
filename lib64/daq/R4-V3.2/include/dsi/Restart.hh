
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
 
#ifndef DSI_RESTART
#define DSI_RESTART

#include "dsi/Frame.hh"

namespace DSI {

class  Restart : public Frame {
public:
  Restart() : Frame(Frame::RESTART) {} 
public:  
  Restart(const Restart&) = delete; 
public:
 ~Restart() {}        
 };

}

#endif

