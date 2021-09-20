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

using namespace DSM::Editor::Path;

/*
** ++
**
**
** --
*/

bool Preference::parse(int  argc, const char* argv[])
 {
 
 DAQ::Path* interface = _interfaces;
 int        remaining = argc;

 if(!remaining--)                   return true;
 if(!_decode(*argv++, interface++)) return false;
 
 if(!remaining--)                   return true;
 if(!_decode(*argv++, interface++)) return false;
 
 if(!remaining--)                   return true;
 return _decode(*argv, interface);
 }

/*
** ++
**
**
** --
*/
 
static const char INVALID[] ="%s is not a valid Path specification\n";
                                               
bool Preference::_decode(const char* arg, DAQ::Path* interface)   
 {
 
 if(arg[0] == '-') return true;
 
 DAQ::Path path(arg);
 
 bool valid = path;
 
 if(valid)
  *interface = path; 
 else
   printf(INVALID, arg);
 
 return valid; 
 }
  
