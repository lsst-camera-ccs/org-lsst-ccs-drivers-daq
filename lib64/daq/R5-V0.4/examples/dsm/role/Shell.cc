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

#include "Set.hh"
#include "Dump.hh"
#include "Reset.hh"
#include "List.hh"

#include "Shell.hh"

using namespace DSM::Editor::Role;
                     
/*
** ++
**
**
** --
*/

Shell::Shell(Interface& interface) : 
 DSM::Editor::Shell(interface)
 {
 
 List*  list  = new List();
 Dump* dump   = new Dump( interface, stack());
 Set*  set    = new Set(  interface, stack());
 Reset* reset = new Reset(interface, stack());

 insert(list);
 insert(dump);
 insert(set);
 insert(reset);
 
 }
