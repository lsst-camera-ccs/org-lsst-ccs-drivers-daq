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

#include "dsm/Source.hh"

#include "Set.hh"

using namespace DSM;
                     
/*
** ++
**
**
** --
*/

static const char COMMAND[] = "set";

SourceEditor::Set::Set(Interface& interface) : 
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

#define SOURCE   argv[0] 
#define LOCATION argv[1] 

static const char INVALID_INDEX[]    = "%d is an invalid index (must be between zero (0) and two (2)\n";
static const char INVALID_SOURCE[]   = "%s is an invalid source specification\n";
static const char INVALID_LOCATION[] = "%s is not a location\n";
 
void SourceEditor::Set::process(int argc, const char** argv, int qualifier)   
 {
 
 if(qualifier > 3) {printf(INVALID_INDEX, qualifier); return;}
 
 DAQ::Interface::Index index = qualifier ? DAQ::Interface::Index(qualifier-1) : DAQ::Interface::Index::ZERO;
  
 DSM::Source source(SOURCE);
 DSI::Location location(LOCATION);
 
 if(!source)   {printf(INVALID_SOURCE, SOURCE);     return;}
 if(!location) {printf(INVALID_LOCATION, LOCATION); return;}
 
 if(!qualifier) 
  _interface.set(source, location);
 else
  _interface.set(source, index, location);

 return; 
 }

/*
** ++
**
**
** --
*/

static const char HELP[] = " TBD (ignore help)\n" 
                           "  This command assigns SCI sources to the interfaces of a specified RCE. The command takes two arguments.\n"
                           "  The first argument corresponds to an SCI location. Each RCE contains three interfaces.\n"
                           "  Interfaces are numbered 0 (zero), 1 (one) and 2 (two).\n"
                           "  Interface numbers are determined by the command's qualifier.\n"
                           "  An SCI location takes the following form:\n"
                           "  The last argument specifies an ATCA location (address) of the RCE connected to the source(s).\n"
                           "  That argument takes the form of 'X/Y/Z'. Where:\n" 
                           "   X: Specifies a slot within the shelf. Slot numbers vary from one (1) to sixteen (16).\n"
                           "   Y: Specfies  a bay number within the slot. Bay numbers vary from  zero (0) to three (3).\n"
                           "   Z: Specifies an RCE within the bay. RCE numbers are either zero (0) or two (2).\n"
                           "  If RCE number is omitted it defaults to zero(0). If bay number is omitted it also defaults\n"
                           "  to zero (0).\n" 
                           "  For example: <set 22 10 5/0/2> assigns to the the RCE at location 5/0/2 to\n" 
                           "  Alternately, <set my_partition 5/0/2>\n" 
                           "  assigns the name to the RCE, again at location 5/0/2.\n"
                           "  Source assignments are transitory until made persistant. Assignments can only be made persistant\n" 
                           "  by restarting the partition. See the <restart> command to restart a partition. To see current\n"
                           "  assignments see the <dump> command. If an assignment is pending (that is, it has been set, but the\n" 
                           "  partition has not yet been restarted), its current assignment will be marked <dirty>. To see any\n" 
                           "  pending assignments see the <dump/pending> commmand.\n"   
                           "\n";
                           
void SourceEditor::Set::help(int argc, const char** argv, int qualifier)   
 {
 
 printf(HELP);

 return;
 }

