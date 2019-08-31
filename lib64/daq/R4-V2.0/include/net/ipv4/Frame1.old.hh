
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
 
#ifndef IPV4_FRAME1
#define IPV4_FRAME1

#include "net/ipv4/Address.hh"
#include "net/link/Frame1.hh"
#include "net/ipv4/Members.hh"
#include "net/udp/Members.hh"

namespace IPV4  {

class  __attribute__((__packed__)) Frame1 : public Link::Frame1 {
public:
 enum {SIZEOF_HEADER   = 128};                                    // Maximum size of XAUII Plugin header (in bytes, but always assumed to be an even number of quadwords)...
 enum {PROTOCOL_LENGTH = sizeof(Link::Frame1) + sizeof(Members)}; // Size of Ethernet frame + IPV4 headers (in bytes, but must be an even number of quadwords)...)
 enum {HEADER_LENGTH   = SIZEOF_HEADER - PROTOCOL_LENGTH};        // Maximum header payload (in bytes, but always assumed to be an even number of quadwords)...
public:
  Frame1()              = delete;
  Frame1(const Frame1&) = delete;
public:
  Frame1(const Address* dst, const Address* src) : Link::Frame1(dst->link(), src->link(), uint16_t(Link::Frame1::IPV4)), ipv4(dst, src) {} 
public:
 ~Frame1() {}
public:
  Members ipv4;
public: 
  unsigned offset()  const {return (unsigned)ipv4.offset() << 3;}               // In bytes...
  unsigned length()  const {return (unsigned)ipv4.length() - sizeof(Members);}  // Less the IPV4 header...
  char*    payload() const {return (char*)&this[1];} 
};

}

#endif


