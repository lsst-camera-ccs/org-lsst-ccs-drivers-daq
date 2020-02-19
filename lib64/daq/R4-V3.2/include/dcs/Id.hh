
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

#ifndef DCS_ID
#define DCS_ID

#include <stdint.h>

namespace DCS {

class __attribute__((__packed__)) Id {
public:
 enum {SIZEOF=16};  // size of ID as string (inclusive of termination...)
public:
 static uint64_t random();
public:
  Id() {} 
  Id(const char* id);                                
  Id(uint64_t id);
  Id(bool)            : _id(uint64_t(0)) {}                                                          
  Id(const Id& clone) : _id(clone._id)   {} 
public:
 ~Id() {} 
public:  
  Id& operator=(const Id& clone) {_id = clone._id;   return *this;}
  Id& operator=(bool posion)     {_id = uint64_t(0); return *this;}
  Id& operator=(uint64_t clone);
public:
  operator bool() const {return _id;}
public:
  uint64_t value() const;
public:
  const char* encode(char* buffer) const;   
public:
  void dump(int indent=0) const;   
private:
  uint64_t _id;  
 };

}

#endif

