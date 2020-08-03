
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
 
#ifndef DSID_SYMBOL
#define DSID_SYMBOL

#include <stdint.h>

namespace DSID {

class __attribute__((__packed__)) Symbol {
public:
  enum {MAX=31}; // exclusive of NULL termination...
public:  
  Symbol()               {_symbol[0] = 0;}  
  Symbol(const char*);
  Symbol(const Symbol&); 
public:
  Symbol& operator=(const Symbol&);
  Symbol& operator=(const char*);  
public:
 ~Symbol() {}        
public:
  operator bool() const {return _symbol[0];} 
public:
  const char* value() const {return _symbol;}
public:  
  unsigned length() const; 
public:
  void empty() {_symbol[0] = 0;} 
private:
  char _symbol[MAX+1];              
 };

}

#endif

