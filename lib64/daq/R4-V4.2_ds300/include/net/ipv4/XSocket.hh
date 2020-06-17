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
 
#ifndef IPV4_XSOCKET
#define IPV4_XSOCKET

#include "net/ipv4/Exception.hh"
#include "net/ipv4/Interface.hh"

#if defined(__rtems__)
#include "src/ipv4/rce/Socket.hh" 
#else
#include "src/ipv4/x86/Socket.hh" 
#endif

namespace IPV4 {

#if defined(__rtems__)

class XSocket : public RCE::Socket {
public:
  XSocket(const Interface& interface, uint16_t port)              throw(Exception) : RCE::Socket(interface, port)         {} 
  XSocket(const Interface& interface, uint16_t port, bool asynch) throw(Exception) : RCE::Socket(interface, port, asynch) {}   
};

//#else

//#include "src/ipv4/x86/NewSocket.hh" 

//class XSocket : public X86::NewSocket {};

#endif

}

#endif