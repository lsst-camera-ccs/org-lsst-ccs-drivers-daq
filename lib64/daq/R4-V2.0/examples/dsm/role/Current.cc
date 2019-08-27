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
                               
RoleEditor::Current::Current() :
 RoleProcessor(),
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

static const char NONE[]  = "<none>";
static const char CLEAN[] = "";
static const char DIRTY[] = "(dirty)";
static const char LINE[]  = " %-8s %-15s " 
                            "%s %s"
                            "\n";
                                  
void RoleEditor::Current::process(const DSI::Location& server, const IPV4::Address& source, const DSM::Role& role, int32_t modified)
 {
 
 char location[16];
 char address[32];
 
 const char* name = role ? role.value() : NONE;
 
 printf(LINE, server.encode(location), source.presentation(address), modified ? DIRTY : CLEAN, name);
  
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

void RoleEditor::Current::summary() const
{

printf(TRAILER, _total);

}