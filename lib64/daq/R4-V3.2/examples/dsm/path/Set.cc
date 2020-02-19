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

#include "dsm/Path.hh"

#include "Set.hh"

using namespace DSM;
                     
/*
** ++
**
**
** --
*/

static const char COMMAND[] = "set";

PathEditor::Set::Set(Interface& interface) : 
 Command(COMMAND),
 _interface(interface)
 {
 }

/*
** ++
**
**
** --
*/

#define PATH     argv[0] 
#define LOCATION argv[1] 

static const char INVALID_INDEX[]    = "%d is an invalid index (must be between zero (0) and two (2)\n";
static const char INVALID_PATH[]     = "%s is an invalid path (must be either 'A' or 'B'\n";
static const char INVALID_LOCATION[] = "%s is not a location\n";
 
void PathEditor::Set::process(int argc, const char** argv, int qualifier)   
 {
 
 if(qualifier > 3) {printf(INVALID_INDEX, qualifier); return;}
 
 DAQ::Interface::Index index = qualifier ? DAQ::Interface::Index(qualifier-1) : DAQ::Interface::Index::ZERO;
  
 DSM::Path path(PATH);
 DSI::Location location(LOCATION);
 
 if(!path)     {printf(INVALID_PATH, PATH);         return;}
 if(!location) {printf(INVALID_LOCATION, LOCATION); return;}
 
 if(!qualifier) 
  _interface.set(path, location);
 else
  _interface.set(path, index, location);

 return; 
 }

/*
** ++
**
**
** --
*/

static const char HELP[] = " TBD (ignore help)\n" 
                           "  This command assigns RCEs to a specified partition. The command takes at least one argument.\n"
                           "  That argument corresponds to a partition name. Partition names can have as many as 31\n"
                           "  characters and the name must have been previously registered with the dsid_editor.\n" 
                           "  All subsequent arguments specify the ATCA locations (addresses) of one or more RCEs. The sum\n"
                           "  of these arguments define the set of RCEs assigned to the partition named by the first argument.\n" 
                           "  Each argument takes the form of 'X/Y/Z'. Where:\n" 
                           "   X: Specifies a slot within the shelf. Slot numbers vary from one (1) to sixteen (16).\n"
                           "   Y: Specfies  a bay number within the slot. Bay numbers vary from  zero (0) to three (3).\n"
                           "   Z: Specifies an RCE within the bay. RCE numbers are either zero (0) or two (2).\n"
                           "  If RCE number is omitted it defaults to zero(0). If bay number is omitted it also defaults\n"
                           "  to zero (0). If NO arguments are provided all RCEs are selected.\n" 
                           "  For example: <set my_partition 10 5/0/2> assigns the RCEs at locations from 10/0/0 to 10/3/2 and\n" 
                           "  as well 5/0/2 to the the partition named 'my_partition'. Alternately, <set my_partition 5/0/2>\n" 
                           "  assigns the name to the single RCE at location 5/0/2.\n"
                           "  Partition assignments are transitory until made persistant. Assignments can only be made persistant\n" 
                           "  by restarting the partition. See the <restart> command to restart a partition. To see current\n"
                           "  assignments see the <dump> command. If an assignment is pending (that is, it has been set, but the\n" 
                           "  partition has not yet been restarted), its current assignment will be marked <dirty>. To see any\n" 
                           "  pending assignments see the <dump/pending> commmand.\n"   
                           "\n";

void PathEditor::Set::help(int argc, const char** argv, int qualifier)   
 {
 
 printf(HELP);

 return;
 }

