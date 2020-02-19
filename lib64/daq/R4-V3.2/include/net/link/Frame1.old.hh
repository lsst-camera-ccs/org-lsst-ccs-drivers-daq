
/*
**  Package:
**	
**
**  Abstract:
**      
**
** arm-rtems4.11-g++ -c net/src/ipv4/Socket.cc -Wno-psabi -fno-zero-initialized-in-bss -fPIC -march=armv7-a -mtune=cortex-a9 -mfpu=neon -mfloat-abi=softfp -DEXPORT=
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
 
#ifndef LINK_FRAME1
#define LINK_FRAME1

#include "net/link/Members.hh"
#include "net/link/Address.hh"

namespace Link {

class __attribute__((__packed__)) Frame1  {
public:
  enum {ARP=0x806, IPV4=0x800}; 
public:
  Frame1(const Address& src,                     uint16_t type=IPV4) : members(     src, type) {} 
  Frame1(const Address& dst, const Address& src, uint16_t type=IPV4) : members(dst, src, type) {} 
  Frame1(const Frame1& clone)                                        : members(clone.members)  {} 
public:
 ~Frame1() {}
private: 
  uint16_t _pad;
public:
  Members members;
public:  
  unsigned header_length()  const {return 128 - sizeof(Members);}  
};

}

#endif



