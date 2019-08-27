/*
** ++
**  Package:
**	
**
**  Abstract:
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

#ifndef DSM_MOUNTPOINTEDITOR_PENDING
#define DSM_MOUNTPOINTEDITOR_PENDING

#include "net/ipv4/Address.hh"
#include "dsi/Location.hh"
#include "dsm/MountpointProcessor.hh"

namespace DSM {namespace MountpointEditor {

class Pending : public DSM::MountpointProcessor { 
public:
  Pending();
public:
 ~Pending() {}
public:
  void process(const DSI::Location&, const IPV4::Address&, const DSM::Mountpoint&, int32_t modified);
public:  
  void summary() const;
private:
  int _total; 
};

}}

#endif

