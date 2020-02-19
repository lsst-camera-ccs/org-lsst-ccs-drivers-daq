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

#ifndef DSM_PARTITIONEDITOR_CURRENT
#define DSM_PARTITIONEDITOR_CURRENT

#include "net/ipv4/Address.hh"
#include "dsi/Location.hh"
#include "dsm/PartitionProcessor.hh"

namespace DSM {namespace PartitionEditor {

class Current : public DSM::PartitionProcessor { 
public:
  Current();
public:
 ~Current() {}
public:
  void process(const DSI::Location&, const IPV4::Address&, const DSM::Partition&, int32_t modified);
public:  
  void summary() const;
private:
  int _total; 
};

}}

#endif

