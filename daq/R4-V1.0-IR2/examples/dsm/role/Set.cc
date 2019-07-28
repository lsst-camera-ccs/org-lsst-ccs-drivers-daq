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

using namespace DSM;
                     
/*
** ++
**
**
** --
*/

static const char COMMAND[] = "set";

RoleEditor::Set::Set(Interface& interface, Shell& shell) : 
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

#include "dsi/LocationSet.hh"
#include "dsm/Role.hh"
#include "dsm/Roles.hh"

#define ROLE argv[0] 

static const char INVALID_ROLE[]      = "%s is not a valid role name (see the <list> command for valid names)\n";
static const char INVALID_LOCATIONS[] = "Starting from the second argument one or more arguments are not valid locations\n";
 
void RoleEditor::Set::process(int argc, const char* argv[], int qualifier)   
 {
 
 if(argc < 2) return;
 
 Roles roles;
 
 if(!roles.lookup(ROLE)) {printf(INVALID_ROLE, ROLE); return;}

 DSI::LocationSet locations(argc-1, &argv[1]);
 
 if(!locations) {printf(INVALID_LOCATIONS); return;}
 
 DSI::LocationSet missing;
  
 bool success = _interface.set(Role(ROLE), locations, missing);

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
                           "  This command assigns RCEs to a specified role. The command takes at least one argument.\n"
                           "  That argument corresponds to the name of a role. See the <list> command for the allowed roles.\n"
                           "  All subsequent arguments specify the ATCA locations (addresses) of one or more RCEs. The sum\n"
                           "  of these arguments define the set of RCEs assigned to the role named by the first argument.\n" 
                           "  Each argument takes the form of 'X/Y/Z'. Where:\n" 
                           "   X: Specifies a slot within the shelf. Slot numbers vary from one (1) to sixteen (16).\n"
                           "   Y: Specfies  a bay number within the slot. Bay numbers vary from  zero (0) to three (3).\n"
                           "   Z: Specifies an RCE within the bay. RCE numbers are either zero (0) or two (2).\n"
                           "  If RCE number is omitted it defaults to zero(0). If bay number is omitted it also defaults\n"
                           "  to zero (0). If NO arguments are provided all RCEs are selected.\n" 
                           "  For example: <set readout 10 5/0/2> assigns the RCEs at locations from 10/0/0 to 10/3/2 and\n" 
                           "  as well 5/0/2 to a 'readout' role'. Alternately, <set emulation 5/0/2>\n" 
                           "  assigns the single RCE at location 5/0/2 to an 'emulation' role.\n"
                           "  Note: If you specify an RCE outside the range of your partition it will be quietly discarded.\n"
                           "  Role assignments are transitory until made persistant. Assignments can only be made persistant\n" 
                           "  by restarting the partition. See the <restart> command to restart a partition. To see current\n"
                           "  assignments see the <dump> command. If an assignment is pending (that is, it has been set, but the\n" 
                           "  partition has not yet been restarted), its current assignment will be marked <dirty>. To see any\n" 
                           "  pending assignments see the <dump/pending> commmand. To de-assign roles see the <reset> command.\n"   
                           "\n";

void RoleEditor::Set::help(int argc, const char** argv, int qualifier)   
 {
 
 printf(HELP);

 return;
 }

