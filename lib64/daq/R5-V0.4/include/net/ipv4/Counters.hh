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
**	000 - January 09, 2007
**
**  Revision History:
**	None.
**
** --
*/
 
#ifndef IPV4_COUNTERS
#define IPV4_COUNTERS

#include <stdint.h>

namespace IPV4 {

class Counters {
public:
  Counters();
  Counters(const Counters&);
public:
 ~Counters() {}
public:
  const Counters& operator=(const Counters&); 
public:                     
  uint32_t received;      // Frames received (less corrupted & re-handled)...
  uint32_t corrupted;     // Discarded due to checksum error
  uint32_t rejected;      // Discarded due to not an acceptable address
  uint32_t rehandled;     // Frames not  accepted and passed back to default handler
  uint32_t datagrams;     // UDP Frames accepted and assembled
  uint32_t fragments;     // Number of UDP datagram fragments   
  uint32_t reclaimed;     // Attempts to reclaim oldest "pending assembly" datagram as heap was dropped,
  uint32_t dropped;       // Rejected by socket
  uint32_t aged_out;      // Removed and deallocated old datagram "pending assembly".
  uint32_t fixed_up;      // Fixed up received invalid "split" datagram, due to a bug in LINUX "sendmsg"  
public:
  void dump(int indent=0) const;  
};

}

#endif
