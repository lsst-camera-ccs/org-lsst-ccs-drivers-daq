
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
 
#ifndef DSI_CLIENT
#define DSI_CLIENT

#include "dvi/Version.hh"
#include "net/ipv4/Address.hh"
#include "net/ipv4/Socket.hh"
#include "dsi/Set.hh"
#include "dsi/Frame.hh"
#include "dsi/Location.hh"

namespace DSI {

class Client {
public:
 enum {TIMEOUT = (10000 * 4)}; // default in 10 us tics (4 milliseconds)... 
 //enum {TIMEOUT = (10000 * 40)}; // default in 10 us tics (40 milliseconds)... 
public:
  Client()              = delete;
  Client(const Client&) = delete;     
public:
  Client(const IPV4::Address& service, const char* interface, bool probe=true);     
public:
 ~Client();
public:  
  const IPV4::Address& address()   const {return _socket.address;}
  const IPV4::Address& service()   const {return _service;}
  const Set&           servers()   const {return _servers;}
public:
  bool probe(uint8_t server, Location&, IPV4::Address&, DVI::Version&); 
public:
  void post(Frame&, const IPV4::Address&);
  void post(Frame&,                                        const void* args, int sizeof_args, const IPV4::Address&);
  void post(Frame&, const void* header, int sizeof_header, const void* args, int sizeof_args, const IPV4::Address&);
public:
  void post(Frame& frame)                                                                           {post(frame,                                           service());}
  void post(Frame& frame,                                        const void* args, int sizeof_args) {post(frame,                        args, sizeof_args, service());}
  void post(Frame& frame, const void* header, int sizeof_header, const void* args, int sizeof_args) {post(frame, header, sizeof_header, args, sizeof_args, service());}
public:
  int wait(const Frame& request, Frame& response);
  int wait(const Frame& request, Frame& response, void* returns, int sizeof_returns);
public:
  int wait(const Frame& request, Frame& response, Set&);
  int wait(const Frame& request, Frame& response, void* returns, int sizeof_returns, Set&);
public:  
  int wait(Frame&);
  //int wait(Frame&, void* returns, int sizeof_returns); // deprecated
public:
  void set(int tmo);
public:
  void probe();
private:
  IPV4::Socket  _socket;
  IPV4::Address _service;
  Set           _servers;
  uint32_t      _sequence; 
};
                 
}

#endif

