
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
 
#ifndef PPS_COUNTERS
#define PPS_COUNTERS

#include <stdint.h>

#include "net/ipv4/Counters.hh"
#include "dsi/Location.hh"

namespace PPS  {

class  __attribute__((__packed__)) Counters {
public:
  static const char* banner();
public:
  Counters() {}
  Counters(const DSI::Location&);
  Counters(const DSI::Location&, uint32_t sent, uint32_t received);
  Counters(const DSI::Location&, uint32_t sent, uint32_t received, const IPV4::Counters&);
public:
  Counters(const Counters&) = delete;
public:
 ~Counters() {}
public:  
  //const Counters& operator=(const Counters& clone) {return *this;} 
public:
  uint32_t sent()             const;
  uint32_t received()         const;
  uint32_t frames_received()  const;
  uint32_t frames_corrupted() const;
  uint32_t frames_rejected()  const;
  uint32_t frames_rehandled() const;
  uint32_t datagrams()        const;
  uint32_t fragments()        const;
  uint32_t reclaimed()        const;     
  uint32_t dropped()          const;     
  uint32_t aged_out()         const;      
  uint32_t fixed_up()         const;     
public:
  const DSI::Location& location() const {return _location;}
public:
  void dump(int indent=0) const;
private:
  DSI::Location _location;
  uint8_t       _pad[3];
private:  
  uint32_t _sent;             // Datagrams sent;
  uint32_t _received;         // Datagrams received
  uint32_t _frames_received;  // Frames received...
  uint32_t _frames_corrupted; // Discarded due to checksum error
  uint32_t _frames_rejected;  // Discarded due to not an acceptable address
  uint32_t _frames_rehandled; // Frames not  accepted and passed back to default handler
  uint32_t _datagrams;        // UDP Frames accepted...
  uint32_t _fragments;        // IPV4 Frames accepted...  
  uint32_t _reclaimed;        // Attempts to reclaim oldest "pending assembly" datagram as heap was exhausted,
  uint32_t _dropped;          // Heap is exhausted and reclaim failed (see above). Port has insufficient buffering?
  uint32_t _aged_out;         // Removed and deallocated old datagram "pending assembly".
  uint32_t _fixed_up;         // Fixed up received invalid "split" datagram, due to a bug in LINUX "sendmsg"    
  };
 
}

#endif


