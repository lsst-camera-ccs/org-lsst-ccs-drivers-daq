
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
 
#ifndef RDS_CLIENT
#define RDS_CLIENT

#include "net/ipv4/Address.hh"
#include "dvi/Version.hh"
#include "daq/LocationSet.hh"
#include "daq/Location.hh"
#include "dsi/Location.hh"
#include "dsi/Set.hh"
#include "dsm/Client.hh"
#include "dsm/Exception.hh"
#include "rds/Event.hh"

#include "rms/InstructionList.hh"

//namespace CMS {class Camera;} // forward declaration...

namespace RDS {

class Stats;

class Client {
public:  
  Client()              = delete; 
  Client(const Client&) = delete;
public: 
  Client(const char* partition)                        throw (DSM::Exception);                              
  Client(const char* partition, const char* interface) throw (DSM::Exception);     
public:
  Client(unsigned partition)                         throw (DSM::Exception);                              
  Client(unsigned partition, const char* interface)  throw (DSM::Exception);     
public:
 ~Client() {}
public: 
  const DSI::Set& sources() const {return _client.servers();}
public:
  bool stats(const DAQ::Location&, bool clear, RDS::Stats&, int32_t& error); 
public:  
  bool probe(const DAQ::Location&, DSI::Location&, IPV4::Address&, DVI::Version&); 
public:  
  bool wakeup(const DAQ::LocationSet&, const Event&);
private:
  DSM::Client _client;
  DSI::Set    _not_sources;
  };

}

#endif

