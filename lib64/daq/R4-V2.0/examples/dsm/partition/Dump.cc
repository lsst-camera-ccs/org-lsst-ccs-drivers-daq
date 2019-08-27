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

#include "Shell.hh"
#include "Pending.hh"
#include "Current.hh"

#include "Dump.hh"

using namespace DSM;
                                        
/*
** ++
**
**
** --
*/

static const char COMMAND[] = "dump";

PartitionEditor::Dump::Dump(Interface& interface, Shell& shell) : 
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
 
static const char INVALID_QUALIFIER[] = "qualifier must be either be 'current' or 'pending'\n";
 
void PartitionEditor::Dump::process(int argc, const char** argv, int qualifier)   
 {
 
 if(!qualifier) qualifier = Shell::Value::CURRENT;
  
 bool valid =  (qualifier == Shell::Value::CURRENT) || (qualifier == Shell::Value::PENDING);
 
 if(!valid) {printf(INVALID_QUALIFIER); return;}
 
 DSI::LocationSet missing;
 
 bool success;

 if(qualifier == Shell::Value::CURRENT)
  {
  
  PartitionEditor::Current processor;
  
  success = _interface.get(Interface::State::CURRENT, processor, missing);
  
  processor.summary();
  
  }
 else
  {
  
  PartitionEditor::Pending processor;
  
  success = _interface.get(Interface::State::PENDING, processor, missing);
  
  processor.summary();

  }

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
                           "  This command removes RCES from a partition. The command takes no arguments.\n"
                           "  Partition assignments resets are transitory until made persistant. Removals can only be made persistant\n" 
                           "  by restarting the partition. See the <restart> command to restart a partition. To see current\n"
                           "  assignments see the <dump> command. If a removal is pending (that is, it has been reset, but the\n" 
                           "  partition has not yet been restarted), its current assignment will be marked <dirty>. To see any\n" 
                           "  pending removals see the <dump/pending> commmand.\n"   
                           "\n";

void PartitionEditor::Dump::help(int argc, const char** argv, int qualifier)   
 {
 
 printf(HELP);

 return;
 }

