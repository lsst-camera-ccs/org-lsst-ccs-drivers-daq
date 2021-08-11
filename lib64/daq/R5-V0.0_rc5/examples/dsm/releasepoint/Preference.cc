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

#include "Preference.hh"

using namespace DSM::Editor::Releasepoint;

static void _dump(const DSI::Location&, const char* value, unsigned pending);

/*
** ++
**
**
** --
*/

Preference::Preference() :
 DSM::Releasepoint(),
 _total(0) 
 {
 
 
 return;
 }
 
/*
** ++
**
**
** --
*/
                               
Preference::Preference(const char* value) :
 DSM::Releasepoint(value),
 _total(0) 
 {
 }

/*
** ++
**
**
** --
*/
                                  
void Preference::process(const DSI::Location& server, const char* value, unsigned pending)
 {
 
 _dump(server, value, pending);
 
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

void Preference::summary() const
{

printf(TRAILER, _total);
 
return;
}

/*
** ++
**
**
** --
*/

static const char NA[]   = "N/A";
static const char YES[]  = "Yes";
static const char NO[]   = "No";
static const char LINE[] = " %-8s %-8s %s\n";
                                  
void _dump(const DSI::Location& server, const char* value, unsigned pending)
 {
 
 char location[16];
 
 printf(LINE, server.encode(location), pending  ? YES : NO, *value ? value : NA);
     
 return;
 } 
