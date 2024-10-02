
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
 
#ifndef PPS_CLIENT
#define PPS_CLIENT

#include "net/ipv4/Address.hh"
#include "dsi/Header.hh"
#include "dsi/Location.hh"
#include "dsi/LocationSet.hh"
#include "dsm/Client.hh"
#include "dsm/Exception.hh"
#include "pps/Counters.hh"
#include "pps/Results.hh"

namespace PPS { 

class Client : public DSM::Client {
public:
 enum {MAX_LOOP=0};
 enum {MAX_SIZE=(64*1024)-64};
public:
  Client(const char* paritition);
public:
  Client()              = delete;
  Client(const Client&) = delete;
public:
 ~Client() {}
public:
  const DSI::LocationSet& servers() {return (const DSI::LocationSet&)_servers;} 
public:  
  int loop() const {return _loop;}
  int size() const {return _size;}
public:  
  const IPV4::Address& lookup(const DSI::Location& location) const {return _address[location.index()];}    
public:
  void available(DSI::LocationSet&);
  void paired(DSI::Location locations[]); 
public:  
  void loopIs(int value) {_loop = value;}
  void sizeIs(int value) {_size = ((value + 1) & ~1);} // Round up to an intergal number of 16-bit words...  
public:
  bool pair(const DSI::Location& ping, const DSI::Location& pong);
public:  
  void reset(const DSI::LocationSet& servers, DSI::LocationSet& accepted);
  void start(const DSI::LocationSet& servers, DSI::LocationSet& accepted, uint8_t verify=0);
  void stop( const DSI::LocationSet& servers, DSI::LocationSet& accepted);
  void dump( const DSI::LocationSet& servers, DSI::LocationSet& accepted, Counters counters[]);
  void dump( const DSI::LocationSet& servers, DSI::LocationSet& accepted, Results  results[]);
public:
  int ping(const DSI::Location&, int loop, unsigned size=0);
private:
  void _wait(const DSI::Header& request, int responses, DSI::LocationSet& accepted);
private:
  int           _loop;
  int           _size;
  IPV4::Address _address[DSI::Set::SIZE + 1];      
};

}

#endif


