
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
 
#ifndef DSI_SOCKET
#define DSI_SOCKET

#include "net/ipv4/Address.hh"
#include "net/ipv4/Socket.hh"
#include "dsi/Frame.hh"
#include "dsi/Location.hh"
#include "dsi/Set.hh"

namespace DSI {

class Socket  {
public:
  enum {MAX=1024*64};
public:
  Socket()              = delete;
  Socket(const Socket&) = delete;
public:
  Socket(                      const char* interface);
  Socket(const IPV4::Address&, const char* interface); 
public:
 ~Socket() {}
public:
  void accept(uint8_t server);
  void accept(const DSI::Location& server);
public:
 void reflect(Frame&);
 void reflect(Frame&, const void* output, int sizeof_output);  
public:
 void reflect(Frame&, uint8_t server);
 void reflect(Frame&, uint8_t server, const void* output, int sizeof_output); 
public:
 void sendTo(const IPV4::Address* destination, const void* content, int sizeof_content) {_socket.sendTo(destination, content, sizeof_content);} 
public:
 void dump(int indent=0) {_socket.address.print(indent);} 
protected:
  int _wait(Frame&, void* input, int sizeof_input=MAX);   
protected:  
  Set _accept;
private:
  IPV4::Socket _socket;
  uint8_t      _server; 
  uint8_t      _pad[3];  
  };
  
}

#endif

