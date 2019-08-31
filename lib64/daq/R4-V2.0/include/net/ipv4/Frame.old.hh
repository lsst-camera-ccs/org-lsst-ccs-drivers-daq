
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
  Frame(const Address* dst, const Address* src) : Link::Frame(dst->link(), src->link(), (uint16_t)Link::Frame::IPV4), ipv4(dst, src) {} 
public:
 ~Frame() {}
public:
  Members ipv4;
public:  
  uint32_t  offset()  const {return ((uint32_t)ipv4.offset() << 3) - (sizeof(IPV4::Members) + sizeof(UDP::Members));}
  uint32_t length()  const {return ipv4.length() - sizeof(IPV4::Members);}
  char*    payload() const {return (char*)&this[1];}    
};

}

#endif


