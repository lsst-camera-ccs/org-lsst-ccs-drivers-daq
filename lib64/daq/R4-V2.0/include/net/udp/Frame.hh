
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
 Frame(const IPV4::Address& dst, const IPV4::Address& src,                  uint16_t length)                : IPV4::Frame(dst, src,         length + sizeof(Members)),                              udp(dst.port(), src.port(), length) {}
 Frame(const IPV4::Address& dst, const IPV4::Address& src, uint8_t  option, uint16_t length)                : IPV4::Frame(dst, src, option, length + sizeof(Members)),                              udp(dst.port(), dst.port(), length) {}
 Frame(const IPV4::Address& dst, const IPV4::Address& src, uint16_t id,     uint16_t length, uint16_t size) : IPV4::Frame(dst, src, id,     length + sizeof(Members), uint16_t(IPV4::Members::MF)), udp(dst.port(), src.port(), size)   {}
public:
~Frame() {} 
public:
 Members udp;
};

}

#endif


