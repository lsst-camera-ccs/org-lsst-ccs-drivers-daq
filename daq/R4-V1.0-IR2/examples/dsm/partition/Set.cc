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

#include "dsm/Partition.hh"

#include "Set.hh"

using namespace DSM;
                     
/*
** ++
**
**
** --
*/

static const char COMMAND[] = "set";

PartitionEditor::Set::Set(Interface& interface, Shell& shell) : 
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

#define PARTITION argv[0] 

static const char INVALID_PARTITION[] = "%s is an invalid partition name\n";
static const char INVALID_LOCATIONS[] = "The second and/or any subsequent arguments are not valid locations\n";
 
void PartitionEditor::Set::process(int argc, const char* argv[], int qualifier)   
 {
 
 if(argc < 2) return;
  
 DSM::Partition   name(PARTITION);
 DSI::LocationSet locations(argc-1, &argv[1]);
 
 if(!name)      {printf(INVALID_PARTITION, PARTITION); return;}
 if(!locations) {printf(INVALID_LOCATIONS);            return;}
 
 DSI::LocationSet missing;
  
 bool success = _interface.set(name, locations, missing);

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
                           
void PartitionEditor::Set::help(int argc, const char** argv, int qualifier)   
 {
 
 printf(HELP);

 return;
 }

