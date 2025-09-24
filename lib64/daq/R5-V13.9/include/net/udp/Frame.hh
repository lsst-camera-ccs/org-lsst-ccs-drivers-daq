
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
 
#ifndef UDP_FRAME
#define UDP_FRAME

#include "net/ipv4/Address.hh"
#include "net/ipv4/Frame.hh"
#include "net/udp/Members.hh"

namespace UDP  {

class __attribute__((__packed__)) Frame : public IPV4::Frame {
public: 
 Frame()             = delete;
 Frame(const Frame&) = delete;
public: 
 Frame(uint16_t length, const IPV4::Address* dst, const IPV4::Address* src) : IPV4::Frame(*dst, *src, length), udp(dst->port(), src->port(), length) {}
public: 
 Frame(const IPV4::Address& dst, const IPV4::Address& src,                  uint16_t length)                                  : IPV4::Frame(dst, src,         length),         udp(dst.port(), src.port(), length) {} // Note: length is *exclusive* of its own IPV4 header...
 Frame(const IPV4::Address& dst, const IPV4::Address& src, uint8_t  option, uint16_t length)                                  : IPV4::Frame(dst, src, option, length),         udp(dst.port(), dst.port(), length) {} // Note: length is *exclusive* of its own IPV4 header...
 Frame(const IPV4::Address& dst, const IPV4::Address& src, uint16_t id,     uint16_t length, uint16_t offset, uint16_t total) : IPV4::Frame(dst, src, id,     length, offset), udp(dst.port(), src.port(), total)  {} // Note: length is *exclusive* of its own IPV4 header...
public:
~Frame() {} 
public: 
  static void* operator new(size_t size, void*     buffer) {return buffer;}
  static void* operator new(size_t size, uint32_t* buffer) {return (void*)buffer;}
public:
 Members udp;
public:
 unsigned  xlength() const {return (unsigned)ipv4.length() - (sizeof(IPV4::Members) + sizeof(UDP::Members));}  // Less the IPV4  and UDP header...
 uint64_t* header() const {return (uint64_t*)&this[1];}  
};

}

#endif


