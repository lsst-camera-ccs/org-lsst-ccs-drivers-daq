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

#ifndef DSM_EDITOR_SOURCE_PREFERENCE
#define DSM_EDITOR_SOURCE_PREFERENCE

#include "daq/Location.hh"

namespace DSM {namespace Editor {namespace Source {

class Preference { 
public:
  Preference() {}
public:
  Preference(const Preference&) = delete; 
public:
 ~Preference() {}
public:
  const DAQ::Location* interfaces() const {return _interfaces;}
public:
  bool parse(int  argc, const char* argv[]);
private:  
  bool _wildcard(int  argc, const char* argv);
private:
  DAQ::Location _interfaces[3];
};

}}}

#endif

