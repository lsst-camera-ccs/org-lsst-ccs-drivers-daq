
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

#include "dsi/Set.hh"
#include "daq/LocationSet.hh"
#include "daq/Location.hh"
#include "dsm/Client.hh"
#include "dsm/Exception.hh"
#include "rds/Event.hh"
#include "rds/Stats.hh"
#include "xds/Counters.hh"

namespace RDS {

class Client : public DSM::Client {
public: 
  Client(const char* partition);                              
  Client(uint8_t     partition);                              
public:
  Client(const char* partition, const char* interface);     
  Client(uint8_t     partition, const char* interface);    
public:  
  Client()              = delete; 
  Client(const Client&) = delete;
public:
 ~Client() {}
public:
  const DAQ::LocationSet& sources() {return (const DAQ::LocationSet&)_servers;}
public:
  bool stats(const DAQ::Location&, bool clear, Stats&,         int32_t& error);
  bool   xds(const DAQ::Location&, bool clear, XDS::Counters&, int32_t& error);
public:  
  bool wakeup(const DAQ::LocationSet&, const Event&);
public:
  bool flush(const DAQ::Location&, int32_t& error);
public:  
  const char* encode(uint8_t index, char buffer[]);
private:
  DSI::Set _not_sources;
  };

}

#endif

