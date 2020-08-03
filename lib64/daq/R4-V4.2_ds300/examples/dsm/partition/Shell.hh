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
 
#ifndef DSM_PARTITIONEDITOR_SHELL
#define DSM_PARTITIONEDITOR_SHELL

#include "osa/Command.hh"
#include "dsm/Interface.hh"
#include "dsm/Editor.hh"

namespace DSM {namespace PartitionEditor {

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
