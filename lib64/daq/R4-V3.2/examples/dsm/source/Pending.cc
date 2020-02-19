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
                             "Location     Address       Index-0      Index-1      Index-2"
                             "\n"
                             "-------- --------------- ------------ ------------ ------------" 
                             "\n";     
                               
SourceEditor::Pending::Pending() :
 SourceProcessor(),
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

static const char ND[]    = "        N/D";
static const char BLANK[] = "           ";
static const char LINE[] = "%-8s" 
                           " %-15s" 
                           " %12s"
                           " %12s"
                           " %12s"
                           "\n";
                                  
void SourceEditor::Pending::process(const DSI::Location& server, const IPV4::Address& source, const DSM::Source sources[], int32_t modified)
 {
 
 char location[16];
 char address[32];

 const char* value0 = sources[0] ? sources[0].value() : ND;
 const char* value1 = sources[1] ? sources[1].value() : ND;
 const char* value2 = sources[2] ? sources[2].value() : ND;
  
 printf(LINE, 
        server.encode(location),
        source.presentation(address),
        modified & (1 << 0) ? value0 : BLANK,
        modified & (1 << 1) ? value1 : BLANK,
        modified & (1 << 2) ? value2 : BLANK
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

void SourceEditor::Pending::summary() const
{

printf(TRAILER, _total);

}