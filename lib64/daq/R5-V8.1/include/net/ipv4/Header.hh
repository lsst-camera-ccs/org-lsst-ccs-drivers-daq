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

#ifndef IPV4_HEADER
#define IPV4_HEADER

#include "net/ipv4/Descriptor.hh"

namespace IPV4 {

class Header : public Descriptor {
public:
 Header()                              : Descriptor()                 {}
 Header(void* location)                : Descriptor(location)         {}
 Header(size_t length)                 : Descriptor(length)           {}
 Header(void* location, size_t length) : Descriptor(location, length) {}
 Header(const Header& clone)           : Descriptor(clone)            {} 
public:
 ~Header() {}
};

}

#endif