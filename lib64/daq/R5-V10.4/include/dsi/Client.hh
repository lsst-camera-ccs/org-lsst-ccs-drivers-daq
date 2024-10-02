
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
#include "net/ipv4/Rundown.hh"
#include "net/ipv4/Datagram.hh"
#include "net/ipv4/Exception.hh"
#include "net/ipv4/Address.hh"
#include "net/ipv4/Counters.hh"

#include "dsi/Socket.hh"
#include "dsi/Address.hh"
#include "dsi/Set.hh"
#include "dsi/Location.hh"
#include "dsi/LocationSet.hh"
#include "dsi/Header.hh"

namespace DSI {

class Client : public Socket {
public:
  enum {TIMEOUT = 4}; // Default wait timeout in 10 ms tics (i., e., 40 milliseconds)... 
public:
  Client(const Address& service, const char* interface, unsigned tmo=TIMEOUT);     
  Client(const Address& service, const char* interface, const Set&, unsigned tmo=TIMEOUT);     
public:
  Client()              = delete;
  Client(const Client&) = delete;     
public:
 ~Client();
public:
  bool post_flush(const Header& header, const IPV4::Address& destination, void* payload, size_t payload_length, IPV4::Rundown* rundown=(IPV4::Rundown*)0) {_flush(payload, payload_length); return post(header, destination, payload, payload_length, rundown);}
public:
  bool post(const Header&, const IPV4::Address&,                                       IPV4::Rundown* rundown=(IPV4::Rundown*)0); 
  bool post(const Header&, const IPV4::Address&, void* payload, size_t payload_length, IPV4::Rundown* rundown=(IPV4::Rundown*)0);
public:
  bool post(const Header& request,                                       IPV4::Rundown* rundown=(IPV4::Rundown*)0) {return post(request, _service,                          rundown);}  
  bool post(const Header& request, void* payload, size_t payload_length, IPV4::Rundown* rundown=(IPV4::Rundown*)0) {return post(request, _service, payload, payload_length, rundown);}   
public:
  IPV4::Datagram* wait(uint32_t      request);  
  IPV4::Datagram* wait(const Header& request) {return wait(request.sequence());}
  IPV4::Datagram* wait()                      {return _wait();}  
public:  
  const Address& service() const {return _service;}
public:  
  virtual const char* encode(uint8_t index, char buffer[]);
public:
  bool probe(Location      vector[]);
  bool probe(IPV4::Address vector[]);
  bool probe(DVI::Version  vector[]);
public:
  bool stats(IPV4::Counters vector[]);
public:  
  bool restart();  
  bool shutdown();
public:
  virtual bool reinit();
private:
  void _finalize_servers();
private:  
  bool _lookup(const Header& request, Location vector[],      Set& remaining); 
  bool _lookup(const Header& request, IPV4::Address vector[], Set& remaining); 
  bool _lookup(const Header& request, DVI::Version vector[],  Set& remaining);
private:
  bool _stats(const Header& request, IPV4::Counters vector[], Set& remaining);
protected:
  Set _servers;
public:   
  const Set& partition_set() const {return _servers;}
private:
  Address  _service;
  uint32_t _sequence;
  }; 
                 
}

#endif

