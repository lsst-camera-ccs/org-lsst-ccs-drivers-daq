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

using namespace DSM::Editor::Path;
                     
/*
** ++
**
**
** --
*/

static const char COMMAND[] = "set";

Set::Set(Interface& interface, DSI::Stack& stack) : 
 Command(COMMAND),
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

#include "dsi/LocationSet.hh"

#include "Preference.hh"

void Set::process(int argc, const char* argv[], int modify)   
 {
 
 Preference preference;

 if(!preference.parse(argc, argv)) return;
 
 DSI::LocationSet missing;
   
 bool success = _interface.path(_stack.servers(), preference.interfaces(), missing);
  
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
                           "  This command assigns paths to one or more of the Front-End interfaces of a specified RCE.\n"
                           "  The command takes from one to three arguments. These arguments correspond to a list of paths,\n" 
                           "  each argument defining the path assigned to one of an RCE's three interfaces. The first\n" 
                           "  argument is the path assigned to Interface-1, the second to Interface-2 and the third to\n" 
                           "  Interface-3. If an argument is omitted its corresponding path is unchanged. A path can have\n"
                           "  one of four values:\n"
                           "   A (either upper or lower case) - Specifies an interface is connected to its corresponding SCI\n"
                           "                                    SCI through the telescope's primary path\n" 
                           "   B (either upper or lower case) - Specifies an interface is connected to its corresponding SCI\n"
                           "                                    SCI through the telescope's redundant path\n" 
                           "   C (either upper or lower case) - Specifies an interface is connected to its corresponding SCI\n"
                           "                                    through the observatory's white-room path\n" 
                           "   I (either upper or lower case) - Specifies an interface is connected to its corresponding SCI\n"
                           "                                    through the DAQ system's internal path (to its emulator).\n" 
                           "  If an interface assignment is to be ignored its corresponding argument is a dash (-). For\n"
                           "  example: <set A a A> sets all three interfaces to their primary batch while <set - I - sets\n" 
                           "  only Interface-1 to its internal path. Note the dashes which signify the values of Interface-1\n" 
                           "  and Interface-3 will remain unchanged.\n"   
                           "  Assignments are transitory until made persistant. Assignments can only be made persistant by\n" 
                           "  restarting the partition. See the 'restart' command to restart a partition. To deassign interface\n" 
                           "  assignments see the 'reset' command. To see either current or pending assignments see the 'dump'\n" 
                           "  command.\n"   
                           "\n";
                                                                            
void Set::help(int argc, const char** argv, int qualifier)   
 {
 
 printf(HELP);
 
 return;
 }

