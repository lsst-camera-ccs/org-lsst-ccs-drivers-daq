
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
 
#ifndef UDP_FRAME1
#define UDP_FRAME1

#include "net/udp/Members.hh"
#include "net/ipv4/Frame1.hh"
#include "net/link/Address.hh"
#include "net/ipv4/Address.hh"

namespace UDP  {

class __attribute__((__packed__)) Frame1 : public IPV4::Frame1 {
public: 
 Frame1()              = delete;
 Frame1(const Frame1&) = delete;
public: 
 Frame1(uint16_t size, const IPV4::Address* dst, const IPV4::Address* src) : IPV4::Frame1(dst, src), udp(dst->port(), src->port(), size) {}
public:
 ~Frame1() {} 
public:
  Members udp;
};

}

#endif


