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

#ifndef DSM_PARTITIONEDITOR_PENDING
#define DSM_PARTITIONEDITOR_PENDING

#include "net/ipv4/Address.hh"
#include "dsi/Location.hh"
#include "dsm/PartitionProcessor.hh"

namespace DSM {namespace PartitionEditor {

class Pending : public DSM::PartitionProcessor { 
public:
  Pending();
public:
 ~Pending() {}
public:
  void process(const DSI::Location&, const IPV4::Address&, const DSM::Partition&, int32_t modified);
public:  
  void summary() const;
private:
  int _total; 
};

}}

#endif

