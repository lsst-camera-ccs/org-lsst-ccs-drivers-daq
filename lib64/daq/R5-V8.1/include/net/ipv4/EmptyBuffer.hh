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
 
#ifndef IPV4_EMPTYBUFFER
#define IPV4_EMPTYBUFFER

#include <stdint.h>

namespace IPV4 {

class EmptyBuffer : public Buffer {
public:
  EmptyBuffer()                         : Buffer()      {}
  EmptyBuffer(const EmptyBuffer& clone) : Buffer(clone) {}
public:
 ~EmptyBuffer() {}
public:
  bool cached() const {return false;}  
public:
  void* allocate(size_t, const uint64_t opaque_header[]) {return (void*)0;}
};

}

#endif
