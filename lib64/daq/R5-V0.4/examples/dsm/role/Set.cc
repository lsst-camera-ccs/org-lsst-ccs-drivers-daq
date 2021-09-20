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

using namespace DSM::Editor::Role;
                     
/*
** ++
**
**
** --
*/

static const char NAME[] = "set";

Set::Set(Interface& interface, DSI::Stack& stack) : 
 OSA::Command(NAME),
 _interface(interface),
 _stack(stack)
 {
 }

/*
** ++
**
**
** --
*/

static const char INVALID_ARGC[] = "Must provide one argument corresponding to the preference value\n";

#define NAME argv[0]

void Set::process(int argc, const char* argv[], int qualifier)   
 {
 
 if(!argc) return;
  
 DSI::LocationSet missing;
  
 bool success = _interface.role(_stack.servers(), NAME, missing);
  
 if(!success) return;
  
 return; 
 }

/*
** ++
**
**
** --
*/
                           
static const char HELP[] = "\n" 
                           "  Assign roles for the servers currently held by the stack.\n"
                           "  Role assignments are transitory until made persistant. Assignments can only be made persistant\n" 
                           "  by restarting the partition. See the 'restart' command to restart a partition. To see current\n"
                           "  assignments see the 'dump' command. If an assignment is pending (that is, it has been set, but\n" 
                           "  the partition has not yet been restarted), its current assignment will be marked pending. To\n" 
                           "  see any pending assignments see the 'dump/pending' commmand. To de-assign roles see the 'reset'\n"   
                           "  command. See the 'empty' command to empty the stack and the push' command to populate the stack.\n"
                           "\n";
                                                 
void Set::help(int argc, const char** argv, int qualifier)   
 {
 
 printf(HELP);
 
 return;
 }

