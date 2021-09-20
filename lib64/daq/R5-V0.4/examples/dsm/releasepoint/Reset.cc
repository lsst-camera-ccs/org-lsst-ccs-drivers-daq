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

#include "Reset.hh"

using namespace DSM::Editor::Releasepoint;
                     
/*
** ++
**
**
** --
*/

static const char NAME[] = "reset";

Reset::Reset(Interface& interface, DSI::Stack& stack) : 
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

void Reset::process(int argc, const char* argv[], int qualifier)   
 {
  
 DSI::LocationSet missing;
 
 bool success = _interface.releasepoint(_stack.servers(), missing);
  
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
                           "  Deassign the releasepoints for the servers currently held by\n"
                           "  the stack. See the 'empty' command to empty the stack. See the\n"
                           "  'push' command to populate the stack.\n"
                           "\n";
                                                 
void Reset::help(int argc, const char** argv, int qualifier)   
 {
 
 printf(HELP);
 
 return;
 }
