
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
 
#ifndef DSM_RELEASEPOINTPROCESSOR
#define DSM_RELEASEPOINTPROCESSOR

#include "net/ipv4/Address.hh"
#include "dsi/Location.hh"
#include "dsm/Releasepoint.hh"

namespace DSM {

class ReleasepointProcessor  {
public:
  ReleasepointProcessor() {}  
public:
  virtual ~ReleasepointProcessor() {}
public: 
  virtual void process(const DSI::Location&, const IPV4::Address&, const DSM::Releasepoint&, int32_t modified) = 0;  
 };

}

#endif

