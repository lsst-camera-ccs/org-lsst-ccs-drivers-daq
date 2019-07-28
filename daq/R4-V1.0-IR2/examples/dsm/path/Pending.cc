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

#include "Pending.hh"

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
                               
PathEditor::Pending::Pending() :
 PathProcessor(),
 _total(0) 
 {
 
 printf(HEADER);
  
 return;
 }

/*
** ++
**
**
** --
*/

static const char BLANK[] = "        ";
static const char LINE[] = " %-8s %-15s " 
                           "%9s "
                           "%9s "
                           "%9s "
                           "\n";
                                  
void PathEditor::Pending::process(const DSI::Location& server, const IPV4::Address& source, const DSM::Path path[], int32_t modified)
 {
 
 char location[16];
 char address[32];
 
 printf(LINE, 
        server.encode(location),
        source.presentation(address),
        modified & (1 << 0) ? path[0].value() : BLANK,
        modified & (1 << 1) ? path[1].value() : BLANK,
        modified & (1 << 2) ? path[2].value() : BLANK
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

void PathEditor::Pending::summary() const
{

printf(TRAILER, _total);

}