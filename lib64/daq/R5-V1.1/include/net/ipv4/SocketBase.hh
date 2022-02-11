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
 
#ifndef IPV4_SOCKETBASE
#define IPV4_SOCKETBASE

#include <sys/socket.h> // For socket options...
#include <netinet/in.h> // For socket IP options...

#include "net/ipv4/Address.hh"
#include "net/ipv4/Interface.hh"
#include "net/ipv4/Exception.hh"
#include "net/ipv4/JoinException.hh"

namespace IPV4 {

class SocketBase {
public: 
  SocketBase(const Interface&)                throw(Exception);  
  SocketBase(const Interface&, uint16_t port) throw(Exception);  
public:
  SocketBase()                  = delete;
  SocketBase(const SocketBase&) = delete; 
public:
  virtual ~SocketBase();
public:
  Address address;
private:
  friend class Socket;
private:     
  void __join(const Address&) throw(JoinException);    
  void __drop(const Address&) throw(JoinException); 
private:    
  uint16_t _bind(uint16_t port) throw(Exception); 
  void     _set(int option);   
private:
  int _socket;
};

}

#endif