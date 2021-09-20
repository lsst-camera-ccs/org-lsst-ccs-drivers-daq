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
 
#ifndef DSM_EDITOR_PARTITION_SHELL
#define DSM_EDITOR_PARTITION_SHELL

#include "dsm/Interface.hh"
#include "../Shell.hh"

namespace DSM {namespace Editor {namespace Partition {

class Shell : public DSM::Editor::Shell { 
public:
  Shell(Interface&);
public:
  Shell()             = delete;
  Shell(const Shell&) = delete;  
};

}}}

#endif
