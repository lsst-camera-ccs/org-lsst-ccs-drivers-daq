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
**	000 - , 2007
**
**  Revision History:
**	None.
**
** --
*/

#ifndef IPV4_PAYLOAD
#define IPV4_PAYLOAD

#include "net/ipv4/Descriptor.hh"

namespace IPV4 {

class Payload : public Descriptor {
public:
 Payload()                              : Descriptor()                 {}
 Payload(void* location)                : Descriptor(location)         {}
 Payload(size_t length)                 : Descriptor(length)           {}
 Payload(void* location, size_t length) : Descriptor(location, length) {}
 Payload(const Payload& clone)          : Descriptor(clone)            {} 
public:
 ~Payload() {}
};

}

#endif