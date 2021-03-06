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
 
#ifndef DSM_MOUNTPOINTEDITOR_SHELL
#define DSM_MOUNTPOINTEDITOR_SHELL

#include "osa/Command.hh"
#include "dsm/Interface.hh"
#include "dsm/Editor.hh"

namespace DSM {namespace MountpointEditor {

class Shell : public DSM::Editor { 
public:
  Shell(Interface&);
public:
 ~Shell() {}
private:
  Interface& _interface;
};

}}

#endif
