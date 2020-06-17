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
                             "Location     Address       Index-0      Index-1      Index-2"
                             "\n"
                             "-------- --------------- ------------ ------------ ------------" 
                             "\n";     
                               
SourceEditor::Current::Current() :
 SourceProcessor(),
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

static const char ND[]    = "N/D";
static const char CLEAN[] = "       ";
static const char DIRTY[] = "(dirty)";

static const char LINE[] = "%-8s" 
                           " %-15s" 
                           " %-7s %4s"
                           " %-7s %4s"
                           " %-7s %4s"
                           "\n";
                                  
void SourceEditor::Current::process(const DSI::Location& server, const IPV4::Address& source, const DSM::Source sources[], int32_t modified)
 {
 
 char location[16];
 char address[32];
 
 const char* value0 = sources[0] ? sources[0].value() : ND;
 const char* value1 = sources[1] ? sources[1].value() : ND;
 const char* value2 = sources[2] ? sources[2].value() : ND;
 
 printf(LINE, 
        server.encode(location),
        source.presentation(address),
        modified & (1 << 0) ? DIRTY : CLEAN, value0,
        modified & (1 << 1) ? DIRTY : CLEAN, value1,
        modified & (1 << 2) ? DIRTY : CLEAN, value2
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

void SourceEditor::Current::summary() const
{

printf(TRAILER, _total);

}