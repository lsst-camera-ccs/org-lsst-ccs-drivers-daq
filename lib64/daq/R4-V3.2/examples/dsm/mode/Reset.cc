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

#include "dsm/Mode.hh"

#include "Reset.hh"

using namespace DSM;
                     
/*
** ++
**
**
** --
*/

static const char COMMAND[] = "reset";

ModeEditor::Reset::Reset(Interface& interface, Shell& shell) : 
 Command(COMMAND),
 _interface(interface),
 _shell(shell)
 {
 }

/*
** ++
**
**
** --
*/
 
void ModeEditor::Reset::process(int argc, const char* argv[], int qualifier)   
 {
    
 DSM::Mode none;

 DSI::LocationSet missing;
  
 bool success = _interface.set(none, missing);

 if(!success) _shell._throw(COMMAND, missing);

 return; 
 }

/*
** ++
**
**
** --
*/

static const char HELP[] = "\n" 
                           "  This command resets a partition's mode. The command takes no arguments.\n"
                           "  Mode resets are transitory until made persistant. Removals can only be made persistant\n" 
                           "  by restarting the partition. See the <restart> command to restart a partition. To see current\n"
                           "  assignments see the <dump> command. If a removal is pending (that is, it has been reset, but the\n" 
                           "  partition has not yet been restarted), its current assignment will be marked <dirty>. To see any\n" 
                           "  pending removals see the <dump/pending> commmand.\n"   
                           "\n";

void ModeEditor::Reset::help(int argc, const char** argv, int qualifier)   
 {
 
 printf(HELP);

 return;
 }

