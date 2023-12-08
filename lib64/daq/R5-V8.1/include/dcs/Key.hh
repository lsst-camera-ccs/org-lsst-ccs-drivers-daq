
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

#ifndef DCS_KEY
#define DCS_KEY

#include <stdint.h>

namespace DCS {

class __attribute__((__packed__)) Key {
public:
  Key() {}
  Key(const char* clone);                                
  Key(const Key& clone) {_key = clone._key;}                               
public:
 ~Key() {} 
public:  
  Key& operator=(const Key& clone) {_key = clone._key; return *this;}
  Key& operator=(uint64_t clone);
public:
  operator bool() const {return _key;}
public:
  uint64_t value() const;
public:
  const char* encode(char* buffer) const;   
public:
  void dump(int ident=0) const;   
private:
   uint64_t _key;  
 };

}

#endif

