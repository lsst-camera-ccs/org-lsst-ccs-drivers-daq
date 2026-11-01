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
**	000 - , 2007
**
**  Revision History:
**	None.
**
** --
*/

#ifndef IPV4_DESCRIPTOR
#define IPV4_DESCRIPTOR

#include <stddef.h>

namespace IPV4 {

class Descriptor {
public:
  Descriptor()                              : _length(0),             _location((void*)0)        {}
  Descriptor(void* location)                : _length(0),             _location(location)        {}
  Descriptor(size_t length)                 : _length(length),        _location((void*)0)        {}
  Descriptor(void* location, size_t length) : _length(length),        _location(location)        {}
  Descriptor(const Descriptor& clone)       : _length(clone._length), _location(clone._location) {}
public:
 ~Descriptor() {}
public:  
  const Descriptor& operator=(const Descriptor& clone) {_length = clone._length; _location = clone._location; return *this;}
public:
  void set(size_t length)   {_length   = length;}
  void set(void*  location) {_location = location;}
public:
  size_t      length()   const {return _length;}
  const void* location() const {return _location;}
private:
  size_t _length;
  void*  _location;   
};

}

#endif
