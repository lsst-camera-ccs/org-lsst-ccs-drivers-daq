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

#ifndef DSM_EDITOR_PATH_PREFERENCE
#define DSM_EDITOR_PATH_PREFERENCE

#include "daq/Path.hh"

namespace DSM {namespace Editor {namespace Path {

class Preference { 
public:
  Preference() {}
public:  
  Preference(const Preference&) = delete; 
public:
 ~Preference() {}
public: 
  bool parse(int  argc, const char* argv[]);
public: 
  const DAQ::Path* interfaces() const {return _interfaces;}
private:  
  bool _decode(const char* arg, DAQ::Path*);   
private:
  DAQ::Path _interfaces[3];
};

}}}

#endif

