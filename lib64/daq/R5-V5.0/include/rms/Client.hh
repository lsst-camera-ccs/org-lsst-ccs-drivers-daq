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
#include "dsi/Set.hh"
#include "daq/LocationSet.hh"
#include "dsm/Client.hh"
#include "dsm/Exception.hh"
#include "rms/InstructionList.hh"
#include "rms/Buffer.hh"
#include "rms/Harvest.hh"
#include "rms/Stats.hh"
#include "daq/InterfaceDriverStats.hh"
#include "daq/InterfaceFirmwareStats.hh"

namespace RMS {

class Client : public DSM::Client {
public:
  Client(const char* partition, const char* interface);
  Client(const char* partition);
  Client(uint8_t     partition, const char* interface);
  Client(uint8_t     partition);
public:
  Client()              = delete;
  Client(const Client&) = delete;
public:
 ~Client() {}
public:
  void access(const DAQ::Location& server,     const InstructionList& input, Harvest& harvest) {_access(DAQ::LocationSet(server), input, harvest);}
  void access(                                 const InstructionList& input, Harvest& harvest) {_access(_servers,                 input, harvest);}
  void access(const DAQ::LocationSet& servers, const InstructionList& input, Harvest& harvest) {_access(servers,                  input, harvest);}
public:
  DAQ::LocationSet reset(const DAQ::Location& server)     {return _reset(DAQ::LocationSet(server));}
  DAQ::LocationSet reset(const DAQ::LocationSet& servers) {return _reset(servers);}
  DAQ::LocationSet reset()                                {return _reset(_servers);}
public:
  bool stats(const DAQ::Location&, bool clear, RMS::Stats&,                  int32_t& error);
  bool stats(const DAQ::Location&, bool clear, DAQ::InterfaceDriverStats&,   int32_t& error);
  bool stats(const DAQ::Location&, bool clear, DAQ::InterfaceFirmwareStats&, int32_t& error);
public:
  const DAQ::LocationSet& sources() const {return (const DAQ::LocationSet&)_servers;}
public:
  const char* encode(uint8_t index, char buffer[]);
private:
  void             _access(const DSI::Set&, const InstructionList&, Harvest&);
  DAQ::LocationSet _reset( const DSI::Set&);
private:
  Payload::Buffer _buffer;
  IPV4::Address   _lut[DSI::Set::SIZE];
};

}

#endif
