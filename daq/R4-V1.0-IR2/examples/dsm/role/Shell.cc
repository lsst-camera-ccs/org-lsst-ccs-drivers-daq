/*
** ++
**  Package:
**	
**
**  Abstract:
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

#include <stdio.h>

#include "List.hh"
#include "Dump.hh"
#include "Set.hh"
#include "Reset.hh"

#include "Shell.hh"

using namespace DSM;
                     
/*
** ++
**
**
** --
*/

RoleEditor::Shell::Shell(Interface& interface) :
 Editor(interface), 
 _interface(interface)
 {
 
 insert(new RoleEditor::List());
 insert(new RoleEditor::Dump(interface, *this));
 insert(new RoleEditor::Set( interface, *this));
 insert(new RoleEditor::Reset( interface, *this));

 }


