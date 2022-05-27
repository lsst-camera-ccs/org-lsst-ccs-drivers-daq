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

#include "Shell.hh"

using namespace DSM::Editor::Mountpoint;
                     
/*
** ++
**
**
** --
*/

Shell::Shell(Interface& interface) : 
 DSM::Editor::Shell(interface)
 {
 
 Dump* dump   = new Dump( interface, stack());
 Set*  set    = new Set(  interface, stack());
 Reset* reset = new Reset(interface, stack());

 insert(dump);
 insert(set);
 insert(reset);
 
 }
