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
 
#ifndef IPV4_BUFFER
#define IPV4_BUFFER

#include <stdint.h>
#include <stddef.h>

namespace IPV4 {

class Buffer {
public:
  Buffer()              {}
  Buffer(const Buffer&) {}
public:
  virtual ~Buffer() {}
public:
  virtual bool cached() const = 0;  
public:
  virtual void* allocate(size_t, const uint64_t opaque_header[]) {return (void*)0;}
};

}

#endif
