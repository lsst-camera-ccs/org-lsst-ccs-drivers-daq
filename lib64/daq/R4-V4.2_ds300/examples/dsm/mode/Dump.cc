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

ModeEditor::Dump::Dump(Interface& interface, Shell& shell) : 
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
 
void ModeEditor::Dump::process(int argc, const char** argv, int qualifier)   
 {
 
 if(!qualifier) qualifier = Shell::Value::CURRENT;
  
 bool valid =  (qualifier == Shell::Value::CURRENT) || (qualifier == Shell::Value::PENDING);
 
 if(!valid) {printf(INVALID_QUALIFIER); return;}
 
 DSI::LocationSet missing;
 
 bool success;

 if(qualifier == Shell::Value::CURRENT)
  {
  
  ModeEditor::Current processor;
  
  success = _interface.get(Interface::State::CURRENT, processor, missing);
  
  processor.summary();
  
  }
 else
  {
  
  ModeEditor::Pending processor;
  
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
                           "  This command prints to standard out for each RCE of its partition (or the freelist), its\n" 
                           "  ATCA location, network address and mode. As all RCE's in a partition must be in the same.\n"
                           "  mode. It is an error if not the case. The command takes no arguments, but however has a\n"
                           "  single qualifier. That qualifier is '/pending'. This qualifier determines whether the\n" 
                           "  output reflects either current or pending assignments. If the qualifier is omitted\n"
                           "  current assignments are printed. For example: <dump> prints the current assignments, while\n"
                           "  <dump/pending> prints any pending assignments.\n" 
                           "  If an assignment is pending (that is, its value has been modified through either a <set>\n" 
                           "  or <reset> command) and current assignments are dumped, its assignment value will be marked\n"
                           "  <dirty>. If an assignment value has not been set or has been reset its value will be marked <N/D>.\n"
                           "\n";

void ModeEditor::Dump::help(int argc, const char** argv, int qualifier)   
 {
 
 printf(HELP);

 return;
 }

