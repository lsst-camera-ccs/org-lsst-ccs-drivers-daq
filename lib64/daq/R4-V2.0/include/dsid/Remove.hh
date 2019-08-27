
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
 
#ifndef DSID_REMOVE
#define DSID_REMOVE

#include "dsi/Frame.hh"
#include "dsid/Symbol.hh"
#include "dsid/Functions.hh"

namespace DSID {

class Remove : public DSI::Frame {
public:
  Remove()                    = delete;
  Remove(const Remove& clone) = delete;
public:
  Remove(const char* name) : Frame(REMOVE), _symbol(name) {}  
public:
 ~Remove() {} 
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

