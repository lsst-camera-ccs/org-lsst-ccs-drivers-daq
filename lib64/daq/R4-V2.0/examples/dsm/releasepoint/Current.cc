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
                             " Location     Address     Current assignment"
                             "\n"
                             " -------- --------------- ------------------" 
                             "\n";     
                               
ReleasepointEditor::Current::Current() :
 ReleasepointProcessor(),
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

static const char ND[]    = "<N/D>";
static const char CLEAN[] = "";
static const char DIRTY[] = "(dirty) ";

static const char LINE[] = " %-8s %-15s " 
                           "%s%s"
                           "\n";
                                  
void ReleasepointEditor::Current::process(const DSI::Location& server, const IPV4::Address& source, const DSM::Releasepoint& releasepoint, int32_t modified)
 {
 
 char location[16];
 char address[32];
 
 const char* value = releasepoint ? releasepoint.value() : ND;
 
 printf(LINE, server.encode(location), source.presentation(address), modified ? DIRTY : CLEAN, value);
  
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

void ReleasepointEditor::Current::summary() const
{

printf(TRAILER, _total);

}