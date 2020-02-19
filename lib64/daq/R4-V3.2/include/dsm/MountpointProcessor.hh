
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
 
#ifndef DSM_MOUNTPOINTPROCESSOR
#define DSM_MOUNTPOINTPROCESSOR

#include "net/ipv4/Address.hh"
#include "dsi/Location.hh"
#include "dsm/Mountpoint.hh"

namespace DSM {

class MountpointProcessor  {
public:
  MountpointProcessor() {}  
public:
  virtual ~MountpointProcessor() {}
public: 
  virtual void process(const DSI::Location&, const IPV4::Address&, const DSM::Mountpoint&, int32_t modified) = 0;  
 };

}

#endif

