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

#include "Capacity.hh"

static const char COMMAND[] = "capacity";

using namespace IMS;

/*
** ++
**
**
** --
*/

Editor::Capacity::Capacity(Store& store) : 
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

static const char VALUE[] = "Total storage capacity is:           %llu pages (%llu bytes)\n";

void Editor::Capacity::process(int argc, const char** argv, int constant)   
 {
 
 DSI::LocationSet missing;
  
 uint64_t capacity = _store.capacity(missing); 
 
 printf(VALUE, capacity, bytes(capacity));
 
 return; 
 }
    
/*
** ++
**
**
** --
*/

static const char HELP[] = "\n"
                           "  Print to standard output the total capacity of the storage system\n"
                           "  (see the <current> command for what capacity remains)\n"
                           "\n";
                              
void Editor::Capacity::help(int argc, const char** argv, int qualifier)   
 {
 
 printf(HELP);
 
 return;
 }
    
