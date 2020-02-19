
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
 
#ifndef DSM_ADDRESS
#define DSM_ADDRESS

#include "dsid/ServiceAddress.hh"
#include "dsm/Services.hh"

namespace DSM {

class  Address : public DSID::ServiceAddress {
public:
  Address()               = delete;
  Address(const Address&) = delete;
public:
  Address(unsigned pid) : DSID::ServiceAddress(unsigned(Services::Id::DSM), pid) {}    
public:
 ~Address() {} 
public: 
  Address& operator=(const Address& clone) {DSID::ServiceAddress::operator=(clone); return *this;}                
 };

}

#endif

