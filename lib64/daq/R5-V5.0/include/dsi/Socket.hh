 
/*
**  Package:
**	
**
**  Abstract:
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
 
#ifndef DSI_SOCKET
#define DSI_SOCKET

#ifdef __rtems__
#include "net/ipv4/rce/Socket.hh"
#else
#include "net/ipv4/x86/Socket.hh"
#endif

#include "net/ipv4/Interface.hh"
#include "net/ipv4/Exception.hh"

#include "dsi/Header.hh"

namespace DSI {

class Socket : public IPV4::Socket {
public:
  enum {TIMEOUT = 1}; // Default wait timeout in 10 ms tics... 
public: 
  Socket(const IPV4::Interface& interface)                              : IPV4::Socket(interface)            {}  
  Socket(const IPV4::Interface& interface, uint16_t port)               : IPV4::Socket(interface, port)      {}  
  Socket(const IPV4::Interface& interface,                unsigned tmo) : IPV4::Socket(interface,       tmo) {}
  Socket(const IPV4::Interface& interface, uint16_t port, unsigned tmo) : IPV4::Socket(interface, port, tmo) {}  
public:
  Socket()              = delete;
  Socket(const Socket&) = delete; 
public:
 ~Socket() {}
private:
  size_t _header(const uint64_t opaque_header[]) const {return ((Header*)opaque_header)->length();}
  }; 
                 
}

#endif

