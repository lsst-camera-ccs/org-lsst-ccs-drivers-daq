
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
 
#ifndef PPS_PAIRING
#define PPS_PAIRING

#include "net/ipv4/Address.hh"
#include "dsi/Location.hh"

namespace PPS {

class  __attribute__((__packed__)) Pairing {
public:
  Pairing()                                                            : _location(),                _address()               {}
  Pairing(const DSI::Location& location, const IPV4::Address& address) : _location(location),        _address(address)        {}
  Pairing(const IPV4::Address& address)                                : _location(),                _address(address)        {}
  Pairing(const DSI::Location& location)                               : _location(location),        _address()               {}
  Pairing(const Pairing& clone)                                        : _location(clone._location), _address(clone._address) {}
public:
 ~Pairing() {}
public:  
  const Pairing& operator=(const Pairing& clone) {_location = clone._location; _address = clone._address; return *this;} 
public:
  operator bool() const {return _location;}
public:  
  const DSI::Location& location() const {return _location;}
  const IPV4::Address& address()  const {return _address;}
private:
  DSI::Location _location;
  uint8_t       _pad[3+4];
  IPV4::Address _address;
  };
  
static_assert(!(sizeof(Pairing) & 7), "Class PPS::Pairing is not an even number of quadwords");

}

#endif


