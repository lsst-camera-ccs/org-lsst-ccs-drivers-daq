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
 
#ifndef DSM_EDITOR_PATH_SHELL
#define DSM_EDITOR_PATH_SHELL

#include "../Shell.hh"
#include "dsm/Interface.hh"

namespace DSM {namespace Editor {namespace Path {

class Shell : public DSM::Editor::Shell { 
public:
  Shell(Interface&);
public:
  Shell()             = delete;
  Shell(const Shell&) = delete;  
};

}}}

#endif
