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
#include "dsm/Modes.hh"

#include "Set.hh"

using namespace DSM;
                     
/*
** ++
**
**
** --
*/

static const char COMMAND[] = "set";

ModeEditor::Set::Set(Interface& interface, Shell& shell) : 
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

#define MODE argv[0] 

static const char INVALID_MODE[] = "%s is an invalid mode\n";
 
void ModeEditor::Set::process(int argc, const char* argv[], int qualifier)   
 {
 
 if(argc < 1) return;
 
 Modes modes;
 
 if(!Mode::valid(modes.lookup(MODE))) {printf(INVALID_MODE, MODE); return;}
  
 DSM::Mode mode(MODE);
 
 DSI::LocationSet missing;
  
 bool success = _interface.set(mode, missing);

 if(!success) _shell._throw(COMMAND, missing);

 return; 
 }

/*
** ++
**
**
** --
*/

/*
** ++
**
**
** --
*/

static const char HELP[] = "\n" 
                           "  This command sets all RCEs of a partition to a specified mode. The command takes a single argument.\n"
                           "  That argument corresponds to the name of a mode to set. See the <list> command for the allowed modes.\n"
                           "  Mode assignments are transitory until made persistant. Assignments can only be made persistant\n" 
                           "  by restarting the partition. See the <restart> command to restart a partition. To see current\n"
                           "  assignments see the <dump> command. If an assignment is pending (that is, it has been set, but the\n" 
                           "  partition has not yet been restarted), its current assignment will be marked <dirty>. To see any\n" 
                           "  pending assignments see the <dump/pending> commmand.\n"   
                           "\n";

void ModeEditor::Set::help(int argc, const char** argv, int qualifier)   
 {
 
 printf(HELP);

 return;
 }

