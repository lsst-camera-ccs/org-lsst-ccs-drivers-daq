
/*
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
**	    000 - April 06, 2011
**
**  Revision History:
**	    None.
**
** --
*/
 
#ifndef DSID_LOOKUP
#define DSID_LOOKUP

#include <string.h>

#include "dsi/Frame.hh"
#include "dsid/Symbol.hh"
#include "dsid/Functions.hh"

namespace DSID {

class Lookup : public DSI::Frame {
public:
  Lookup()                    = delete;
  Lookup(const Lookup& clone) = delete;
public:
  Lookup(const char* name) : Frame(LOOKUP), _symbol(name) {}  
public:
 ~Lookup() {} 
public:
  operator bool() const {return _symbol;}
public: 
  const void* argument() const {return (const void*)&_symbol;}
  int         length()   const {return sizeof(Symbol);}
private:
  Symbol _symbol;  
 };

}

#endif

