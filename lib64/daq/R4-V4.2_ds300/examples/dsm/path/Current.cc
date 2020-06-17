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

#include "Current.hh"

using namespace DSM;
 
/*
** ++
**
**
** --
*/

static const char HEADER[] = "\n"
                             " Location     Address      Index 0   Index 1   Index 2"
                             "\n"
                             " -------- --------------- --------- --------- ---------" 
                             "\n";     
                               
PathEditor::Current::Current() :
 PathProcessor(),
 _total(0) 
 {
 
 printf(HEADER);

 }

/*
** ++
**
**
** --
*/

static const char CLEAN[] = "       ";
static const char DIRTY[] = "(dirty)";

static const char LINE[] = " %-8s %-15s " 
                           "%s %-1s "
                           "%s %-1s "
                           "%s %-1s "
                           "\n";
                                  
void PathEditor::Current::process(const DSI::Location& server, const IPV4::Address& source, const DSM::Path path[], int32_t modified)
 {
 
 char location[16];
 char address[32];
 
 printf(LINE, 
        server.encode(location),
        source.presentation(address),
        modified & (1 << 0) ? DIRTY : CLEAN, path[0].value(),
        modified & (1 << 1) ? DIRTY : CLEAN, path[1].value(),
        modified & (1 << 2) ? DIRTY : CLEAN, path[2].value()
        );
  
 _total++;
   
 return;
 }   
   
/*
** ++
**
**
** --
*/

static const char TRAILER[] = "  total = %d\n";

void PathEditor::Current::summary() const
{

printf(TRAILER, _total);

}