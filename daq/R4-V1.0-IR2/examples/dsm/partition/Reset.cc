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

#include "Reset.hh"

using namespace DSM;
                     
/*
** ++
**
**
** --
*/

static const char COMMAND[] = "reset";

PartitionEditor::Reset::Reset(Interface& interface, Shell& shell) : 
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

static const char INVALID_LOCATIONS[] = "The second and/or any subsequent arguments are not valid locations\n";
 
void PartitionEditor::Reset::process(int argc, const char* argv[], int qualifier)   
 {
 
 if(argc < 1) return;
  
 DSI::LocationSet locations(argc, argv);
 
 if(!locations) {printf(INVALID_LOCATIONS); return;}
 
 DSM::Partition none;

 DSI::LocationSet missing;
  
 bool success = _interface.set(none, locations, missing);

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
                           "  This command removes RCEs from their current partition. The command takes at least one argument.\n"
                           "  These arguments specify the ATCA locations (addresses) of one or more RCEs. The sum of these\n"
                           "  arguments define the set of RCEs to be removed from their partition and returned to the freelist.\n" 
                           "  Any RCE specified by an argument which is not in the partition is ignored.\n"
                           "  Each argument takes the form of 'X/Y/Z'. Where:\n" 
                           "   X: Specifies a slot within the shelf. Slot numbers vary from one (1) to sixteen (16).\n"
                           "   Y: Specfies  a bay number within the slot. Bay numbers vary from  zero (0) to three (3).\n"
                           "   Z: Specifies an RCE within the bay. RCE numbers are either zero (0) or two (2).\n"
                           "  If RCE number is omitted it defaults to zero(0). If bay number is omitted it also defaults\n"
                           "  to zero (0). If NO arguments are provided all RCEs are selected.\n" 
                           "  For example: <reset 10 5/0/2> removes, from its partition the RCEs at locations from 10/0/0 to 10/3/2\n" 
                           "  and as well 5/0/2. Alternately, <reset my_partition 5/0/2> assigns the name to the single RCE at location\n"
                           "  5/0/2.\n"
                           "  Partition removals are transitory until made persistant. Removals can only be made persistant\n" 
                           "  by restarting the partition. See the <restart> command to restart a partition. To see current\n"
                           "  assignments see the <dump> command. If a removal is pending (that is, it has been reset, but the\n" 
                           "  partition has not yet been restarted), its current assignment will be marked <dirty>. To see any\n" 
                           "  pending removals see the <dump/pending> commmand.\n"   
                           "\n";

void PartitionEditor::Reset::help(int argc, const char** argv, int qualifier)   
 {
 
 printf(HELP);

 return;
 }

