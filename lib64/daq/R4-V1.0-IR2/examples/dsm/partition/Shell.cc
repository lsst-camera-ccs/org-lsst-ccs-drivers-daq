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

PartitionEditor::Shell::Shell(Interface& interface) :
 Editor(interface), 
 _interface(interface)
 {
 
 insert(new PartitionEditor::Dump(interface, *this));
 insert(new PartitionEditor::Set( interface, *this));
 insert(new PartitionEditor::Reset( interface, *this));

 }


