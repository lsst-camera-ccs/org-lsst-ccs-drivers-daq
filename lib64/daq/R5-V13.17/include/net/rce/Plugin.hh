/*
** ++
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
**	000 - , 2007
**
**  Revision History:
**	None.
**
** --
*/
 
#ifndef RCE_PLUGIN
#define RCE_PLUGIN

#include <stdint.h>    // appears to be necessary for SAS include to compile...

#include <rtems.h>

#include "sas/Sas.h"

namespace RCE {

class Plugin {
public: 
  Plugin(unsigned sizeof_header);
public:
 ~Plugin();
public:
  void  post(const uint64_t* header); 
  // jgt 2018-12-19
  // I have added the reduce header argument as a temporary hack to allow
  // NET::IPV4::Socket to post IPV4 fragments that don't include the UDP header.
  // This will be fixed by Mike when he revisits the stack.
  void  post(const uint64_t* header,                                                                                                     void* payload, unsigned sizeof_payload, bool more, int reduce_header=0);
  void  post(const uint64_t* header, const uint64_t* header1, unsigned sizeof_header1,                                                   void* payload, unsigned sizeof_payload, bool more); 
  void  post(const uint64_t* header, const uint64_t* header1, unsigned sizeof_header1, const uint64_t* header2, unsigned sizeof_header2, void* payload, unsigned sizeof_payload, bool more); 
public:
  void print(int indent=0) const;
private:
  unsigned    _sizeof_header;
  SAS_Session _session;
  SAS_ObMbx   _mbx; 
//  SAS_Mbx     _wait; 
  uint32_t    _mid;
  rtems_id    _mutex;
};

}

#endif
