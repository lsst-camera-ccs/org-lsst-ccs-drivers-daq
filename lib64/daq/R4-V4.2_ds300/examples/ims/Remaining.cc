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

#include "Remaining.hh"

static const char COMMAND[] = "remaining";

using namespace IMS;

/*
** ++
**
**
** --
*/

Editor::Remaining::Remaining(Store& store) : 
 Command(COMMAND),
 _store(store)
 { 
 }

/*
** ++
**
**
** --
*/

#include "dsi/LocationSet.hh"

static const char VALUE[] = "Total remaining storage capacity is: %llu pages (%llu bytes)\n";

void Editor::Remaining::process(int argc, const char** argv, int constant)   
 {
 
 DSI::LocationSet missing;
  
 uint64_t remaining = _store.remaining(missing); 
 
 printf(VALUE, remaining, bytes(remaining));
 
 return; 
 }
    
/*
** ++
**
**
** --
*/

static const char HELP[] = "\n" 
                           "  Print to standard output the remaining capacity of the storage\n" 
                           "  system (see the <capacity> command for the total capacity)\n"
                           "\n";
                              
void Editor::Remaining::help(int argc, const char** argv, int qualifier)   
 {
 
 printf(HELP);
 
 return;
 }
    
