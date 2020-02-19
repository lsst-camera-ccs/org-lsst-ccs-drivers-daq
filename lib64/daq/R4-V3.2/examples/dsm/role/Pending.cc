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
                             " Location     Address     Pending assignment"
                             "\n"
                             " -------- --------------- ------------------" 
                             "\n";     
                               
RoleEditor::Pending::Pending() :
 RoleProcessor(),
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

static const char NONE[]  = "<none>";
static const char BLANK[] = "        ";
static const char LINE[]  = " %-8s %-15s " 
                            "%s "
                            "\n";
                                  
void RoleEditor::Pending::process(const DSI::Location& server, const IPV4::Address& source, const DSM::Role& role, int32_t modified)
 {
 
 char location[16];
 char address[32];
 
 const char* name = role ? role.value() : NONE;

 printf(LINE, server.encode(location), source.presentation(address), modified  ? name : BLANK);
  
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

void RoleEditor::Pending::summary() const
{

printf(TRAILER, _total);

}