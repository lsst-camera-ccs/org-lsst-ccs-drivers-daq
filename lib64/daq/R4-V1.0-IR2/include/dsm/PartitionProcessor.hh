
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
 
#ifndef DSM_PARTITIONPROCESSOR
#define DSM_PARTITIONPROCESSOR

#include "net/ipv4/Address.hh"
#include "dsi/Location.hh"
#include "dsm/Partition.hh"

namespace DSM {

class PartitionProcessor  {
public:
  PartitionProcessor() {}  
public:
  virtual ~PartitionProcessor() {}
public: 
  virtual void process(const DSI::Location&, const IPV4::Address&, const DSM::Partition&, int32_t modified) = 0;  
 };

}

#endif

