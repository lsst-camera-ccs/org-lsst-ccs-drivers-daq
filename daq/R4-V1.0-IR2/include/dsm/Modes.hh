/*
** ++
**  Package:
**	
**
**  Abstract:
**      
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
 
#ifndef DSM_MODES
#define DSM_MODES

#include "dsm/Mode.hh"

namespace DSM {

class Modes { 
public:
  Modes();
public:  
  Modes(const Modes&) = delete;
public:
 ~Modes() {}
public:
  Mode::Id lookup(const char* mode) const;
  Mode::Id lookup(const Mode& mode) const {return lookup(mode.value());}
public:  
  unsigned all() const {return _all;}
public:  
  const char* name(Mode::Id id) const {return Mode::valid(id) ? _name[id] : (const char*)0;}
private:
  unsigned    _all;
  const char* _name[Mode::Id::INVALID+1];
};

}

#endif
