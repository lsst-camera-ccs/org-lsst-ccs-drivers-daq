
/*
**  Package:
**	
**
**  Abstract:
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
 
#ifndef DSI_BUFFER
#define DSI_BUFFER

#include "net/ipv4/Buffer.hh"

namespace DSI {

class Buffer : public IPV4::Buffer {
public:
 enum {LENGTH = 64*1024};         // Size of any one datagram buffer..
 enum {NUMBER = 64};              // Number of datagram buffers available for allocation... 
 enum {SIZE   = LENGTH * NUMBER}; // Size of region allocated...
public:
  Buffer();
public:
  Buffer(const Buffer&) = delete;     
public:
 ~Buffer();
public:
  bool cached() const {return false;}  
public:
  void* allocate(size_t size, const uint64_t opaque_header[]) {return _allocate();}
protected:
  void* _allocate() {unsigned offset = _next; _next = offset + 1; offset &= NUMBER - 1; offset *= LENGTH; return (void*)(_base + offset);}
private:
  unsigned _next;
  char*    _base;  
  }; 
                 
}

#endif

