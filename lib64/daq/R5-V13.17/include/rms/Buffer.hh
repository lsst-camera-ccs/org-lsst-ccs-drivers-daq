
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
 
#ifndef RMS_PAYLOAD_BUFFER
#define RMS_PAYLOAD_BUFFER

#include "net/ipv4/Buffer.hh"

namespace RMS {namespace Payload {

class Buffer : public IPV4::Buffer {
public:
  enum {SIZE = 512};
public:
  Buffer();
public:
  Buffer(const Buffer&) = delete;     
public:
 ~Buffer() {delete[] _buffer;}
public:
  bool cached() const {return true;}  
public:
  void* allocate(size_t size, const uint64_t header[]);
private:
  void* _allocate(unsigned server) {unsigned offset = server * SIZE; return (void*)(_buffer + offset);}
private:
  char* _buffer;
  }; 
                 
}}

#endif

