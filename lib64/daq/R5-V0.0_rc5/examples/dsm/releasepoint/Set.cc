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

using namespace DSM::Editor::Releasepoint;
                     
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
  
 bool success = _interface.releasepoint(_stack.servers(), NAME, missing);
  
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
                           "  Assign the partition names for the servers currently held by\n"
                           "  the stack. See the 'empty' command to empty the stack. See the\n"
                           "  'push' command to populate the stack.\n"
                           "\n";
                                                 
void Set::help(int argc, const char** argv, int qualifier)   
 {
 
 printf(HELP);
 
 return;
 }

