
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
 
#ifndef DSID_ASSIGN
#define DSID_ASSIGN

#include "dsi/Frame.hh"
#include "dsid/Symbol.hh"
#include "dsid/Functions.hh"

namespace DSID {

class Assign : public DSI::Frame {
public:
  Assign()                    = delete;
  Assign(const Assign& clone) = delete;
public:
  Assign(const char* name) : Frame(ASSIGN), _symbol(name) {}  
public:
 ~Assign() {} 
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

