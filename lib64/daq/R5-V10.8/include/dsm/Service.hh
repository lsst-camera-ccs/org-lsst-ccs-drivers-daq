
/*
**  Package:
**	
**
**  Abstract:
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
 
#ifndef DSM_SERVICE
#define DSM_SERVICE

#include "net/ipv4/Buffer.hh"
#include "dsi/Address.hh"
#include "dsi/Service.hh"
#include "dsi/Location.hh"

namespace DSM {

class Service : public DSI::Service {
public:
  Service(const DSI::Address&, const DSI::Location&, const char* interface);
  Service(const DSI::Address&, const DSI::Location&, const char* interface, IPV4::Buffer*);
public:  
 ~Service() {}
  }; 

}

#endif

