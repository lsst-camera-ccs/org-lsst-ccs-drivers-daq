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

#include "dsm/Modes.hh"
#include "dsm/Mode.hh"

#include "List.hh"

using namespace DSM;
                                       
/*
** ++
**
**
** --
*/

static const char COMMAND[] = "list";

ModeEditor::List::List() : 
 Command(COMMAND)
 {
 }

/*
** ++
**
**
** --
*/

static const char LINE[]    = "  %2u %s\n";
static const char SUMMARY[] = "  Total = %d\n"; 

static const char MODES[] = "  ID Mode...\n"
                            "  -- -------\n";
                            
void ModeEditor::List::process(int argc, const char** argv, int qualifier)   
 {

 printf(MODES);
 
 Modes modes;
 
 unsigned remaining = modes.all();
 unsigned total     = 0;
 unsigned id        = __builtin_ffs(remaining);
  
 while(remaining)
  {
  
  id--;
  
  printf(LINE, id, modes.name(Mode::Id(id)));
 
  total++;
  
  remaining &= ~(1 << id); 
  
  id = __builtin_ffs(remaining);
  }
 
 printf(SUMMARY, total);
 
 return; 
 }

/*
** ++
**
**
** --
*/

static const char HELP[] = "\n" 
                           "  This command prints, to standard out the list of permitted modes.\n"
                           "  See the <set> command to put a partition in a specified mode.\n" 
                           "\n";

void ModeEditor::List::help(int argc, const char** argv, int qualifier)   
 {
 
 printf(HELP);

 return;
 }

