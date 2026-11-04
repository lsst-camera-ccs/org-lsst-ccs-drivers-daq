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

using namespace DSM::Editor::Source;
                     
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

#include "dsi/LocationSet.hh"
#include "Preference.hh"

void Set::process(int argc, const char* argv[], int qualifier)   
 {
 
 Preference preference;

 if(!preference.parse(argc, argv)) return;
 
 DSI::LocationSet missing;
   
 bool success = _interface.source(_stack.servers(), preference.interfaces(), missing);
  
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
                           "  This command assigns SCI sources to one or more of the Front-End interfaces of a specified\n"
                           "  RCE. The command takes either one or three arguments.\n"  
                           "  The one or three arguments correspond to a list of sources, each argument defining the source\n"
                           "  assigned to one of an RCE's three interfaces. The first argument of that list is the source\n" 
                           "  assigned to Interface-1, the second to Interface-2 and the third to Interface-3. A source is\n" 
                           "  specified by its SCI address which takes the following form: 'X/Y'. Where:'\n"
                           "   X corresponds to a camera bay. Bays can have any one of the values 0-4, 10-14, 20-24, 30-34,\n" 
                           "     40-44 or 45.\n"
                           "   Y corresponds to a REB within the specified bay. Each board contains one SCI. Boards are numbered\n"
                           "     from zero (0) to two (2). If the board number is omitted all three boards are assumed. Board\n" 
                           "     numbers can only omitted within the first argument of a list. In such a case all subsequent\n" 
                           "     arguments are ignored.\n" 
                           "  For example: '22/0' specifies the first REB in bay 22 while '22' specifies all three boards of\n"
                           "  the same bay. If an interface is to be ignored its corresponding argument is a dash (-). For\n"
                           "  example: <set 10/0/2 22> sets all three interfaces of the RCE at address 10/0/2 to bay 22.\n" 
                           "  Interface-1 is set to board zero (0), Interface-2 to board one (1) and Interface-3 to board two (2).\n" 
                           "  The same assignment could have also been specified with the command <set 10/0/2 22/0 22/1 22/2>.\n" 
                           "  However, if only Interface-1 should be assigned the command could have been: <set 10/0/2 - 22/1 ->.\n" 
                           "  Note the dashes which signify the values of Interface-1 and Interface-3 will remain unchanged.\n"   
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

