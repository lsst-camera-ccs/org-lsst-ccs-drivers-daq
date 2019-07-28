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
**	000 - January 09, 2007
**
**  Revision History:
**	None.
**
** --
*/
 
#ifndef DSID_SERVICEADDRESS
#define DSID_SERVICEADDRESS

#include "net/ipv4/Address.hh"

namespace DSID {

class __attribute__((__packed__)) ServiceAddress : public IPV4::Address {
public:
  enum {MAX=63};  // Maximum number of services per partition
public:
  ServiceAddress();
  ServiceAddress(const IPV4::Address& address) : IPV4::Address(address) {}
  ServiceAddress(unsigned  service, unsigned partition=32);  
  ServiceAddress(const char* service, const char* partition);  
  ServiceAddress(const ServiceAddress& clone) : IPV4::Address(clone) {}
public:
 ~ServiceAddress() {}
public: 
  ServiceAddress& operator=(const ServiceAddress& clone) { IPV4::Address::operator=(clone); return *this;}       
};

}

#endif
