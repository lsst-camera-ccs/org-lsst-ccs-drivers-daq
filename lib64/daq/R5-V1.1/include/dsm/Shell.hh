
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
 
#ifndef DSM_SHELL
#define DSM_SHELL

#include "dsi/editor/Shell.hh"
#include "dsi/Client.hh"

namespace DSM {

class Shell : public DSI::Editor::Shell {
public:
  Shell(DSI::Client& client, uint8_t pid);
public:
  Shell()             = delete;
  Shell(const Shell&) = delete;
public:
 ~Shell() {}
 };

}

#endif

