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

using namespace DSM::Editor::Source;

static bool _decode(const char* arg, DAQ::Location* interface);   

/*
** ++
**
**
** --
*/

bool Preference::parse(int  argc, const char* argv[])
 {
 
 DAQ::Location* interface = _interfaces;
 int            remaining = argc;

 if(_wildcard(remaining, *argv)) return true;

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

bool Preference::_wildcard(int  argc, const char* arg)
 {
 
 DAQ::Location location(arg);

 if(argc != 1)            return false;
 if(!location.wildcard()) return false;
 
 uint8_t bay = location.bay();
 
 DAQ::Location location0(bay, DAQ::Location::Board::ZERO);
 DAQ::Location location1(bay, DAQ::Location::Board::ONE);
 DAQ::Location location2(bay, DAQ::Location::Board::TWO);
 
 _interfaces[0] = location0;
 _interfaces[1] = location1;
 _interfaces[2] = location2;
 
 return true;
 }

/*
** ++
**
**
** --
*/
 
static const char INVALID[] ="In this context %s is not a valid SCI address\n";
                                               
bool _decode(const char* arg, DAQ::Location* interface)   
 {
 
 if(arg[0] == '-') return true;
 
 DAQ::Location location(arg);
 
 bool valid = !location.wildcard() && location;
 
 if(valid)
  *interface = location; 
 else
   printf(INVALID, arg);
 
 return valid; 
 }


