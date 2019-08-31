 
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

#ifndef RMS_CLIENT
#define RMS_CLIENT

#include "net/ipv4/Address.hh"
#include "dvi/Version.hh"
#include "dsi/Set.hh"
#include "daq/LocationSet.hh"
#include "dsm/Client.hh"
#include "dsm/Exception.hh"
#include "rms/InstructionList.hh"
#include "rms/Harvest.hh"

namespace DAQ { class InterfaceDriverStats; class InterfaceFirmwareStats; }

namespace RMS {

class Stats;

class Client {
public:
  Client()              = delete;
  Client(const Client&) = delete; 
public:  
  Client(const char* partition, const char* interface) throw (DSM::Exception);
  Client(const char* partition)                        throw (DSM::Exception);
public:
 ~Client() {} 
public:
  const DSI::Set& sources() {return _client.servers();} 
public:  
  bool probe(const DAQ::Location&, DSI::Location&, IPV4::Address&, DVI::Version&); 
public: 
  void access(const DAQ::Location&,    const InstructionList&, Harvest&);
  void access(const DAQ::LocationSet&, const InstructionList&, Harvest&);
  void access(                         const InstructionList&, Harvest&);
public:
  DAQ::LocationSet reset(const DAQ::Location&);
  DAQ::LocationSet reset(const DAQ::LocationSet&); 
  DAQ::LocationSet reset(); 
public:
  bool stats(const DAQ::Location&, bool clear, RMS::Stats&, int32_t& error); 
  bool stats(const DAQ::Location&, bool clear, DAQ::InterfaceDriverStats&, int32_t& error); 
  bool stats(const DAQ::Location&, bool clear, DAQ::InterfaceFirmwareStats&, int32_t& error); 
private:
  void _access(const DSI::Set&, const InstructionList&, Harvest&);
  DAQ::LocationSet _reset( const DSI::Set&); 
private:
  DSM::Client _client;
};
  
}

#endif

