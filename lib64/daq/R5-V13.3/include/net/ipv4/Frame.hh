
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
 
#ifndef IPV4_FRAME
#define IPV4_FRAME

#include "net/ipv4/Address.hh"
#include "net/link/Frame.hh"
#include "net/ipv4/Members.hh"
#include "net/udp/Members.hh"

namespace IPV4  {

class  __attribute__((__packed__)) Frame : public Link::Frame {
public:
 enum {SIZEOF_HEADER   = 128};                                   // Maximum size of XAUII Plugin header (in bytes, but always assumed to be an even number of quadwords)...
 enum {PROTOCOL_LENGTH = sizeof(Link::Frame) + sizeof(Members)}; // Size of Ethernet frame + IPV4 headers (in bytes, but must be an even number of quadwords)...)
 enum {HEADER_LENGTH   = SIZEOF_HEADER - PROTOCOL_LENGTH};       // Maximum header payload (in bytes, but always assumed to be an even number of quadwords)...
public:
  Frame()             = delete;
  Frame(const Frame&) = delete;
public:
  Frame(const Address& dst, const Address& src,                  uint16_t length)                  : Link::Frame(dst.link(), src.link()), ipv4(dst, src,         length)         {} // Note: length is *exclusive* of its own header...
  Frame(const Address& dst, const Address& src, uint8_t  option, uint16_t length)                  : Link::Frame(dst.link(), src.link()), ipv4(dst, src, option, length)         {} // Note: length is *exclusive* of its own header...
  Frame(const Address& dst, const Address& src, uint16_t id,     uint16_t length, uint16_t offset) : Link::Frame(dst.link(), src.link()), ipv4(dst, src, id,     length, offset) {} // Note: length is *exclusive* of its own header...
public:
 ~Frame() {}
public: 
  static void* operator new(size_t size, void*     buffer) {return buffer;}
  static void* operator new(size_t size, uint32_t* buffer) {return (void*)buffer;}
public:
  Members ipv4;
public: 
  unsigned  offset() const {return (unsigned)ipv4.offset() << 3;}               // In bytes...
  unsigned  length() const {return (unsigned)ipv4.length() - sizeof(Members);}  // Less the IPV4 header...
  uint64_t* header() const {return (uint64_t*)&this[1];} 
};

}

#endif


