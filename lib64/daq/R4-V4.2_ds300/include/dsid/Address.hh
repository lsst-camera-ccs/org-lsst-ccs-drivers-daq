
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
 
#ifndef DSID_ADDRESS
#define DSID_ADDRESS

#include "dsid/ServiceAddress.hh"

namespace DSID {

class Address : public ServiceAddress {
public:
  Address(unsigned service=0)   : ServiceAddress(service, 32) {}  
  Address(const Address& clone) : ServiceAddress(clone) {} 
public:
 ~Address() {} 
public: 
  Address& operator=(const Address& clone) {ServiceAddress::operator=(clone); return *this;}                  
 };

}

#endif

