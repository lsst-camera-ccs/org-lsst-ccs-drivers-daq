
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
 
#ifndef DSM_ROLEPROCESSOR
#define DSM_ROLEPROCESSOR

#include "net/ipv4/Address.hh"
#include "dsi/Location.hh"
#include "dsm/Role.hh"

namespace DSM {

class RoleProcessor  {
public:
  RoleProcessor() {}  
public:
  virtual ~RoleProcessor() {}
public: 
  virtual void process(const DSI::Location&, const IPV4::Address&, const DSM::Role&, int32_t modified) = 0;  
 };

}

#endif

