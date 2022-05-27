
/*
**  Package:
**	
**
**  Abstract:
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
 
#ifndef EMU_SHELL
#define EMU_SHELL

#include "ims/Shell.hh"
#include "ims/Store.hh"
#include "emu/PlayList.hh"

namespace EMU {

class Shell : public IMS::Shell {
public:
  Shell(IMS::Store&, const char* folder, PlayList* = (PlayList*)0);
public:
 ~Shell() {}
public:
  virtual void announce();  
  }; 
                 
}

#endif
