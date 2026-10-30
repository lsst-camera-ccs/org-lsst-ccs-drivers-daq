
#ifndef RMS_CLIENT
#define RMS_CLIENT

#include "net/ipv4/Address.hh"
#include "dsi/Set.hh"
#include "daq/LocationSet.hh"
#include "dsm/Client.hh"
#include "dsm/Exception.hh"
#include "rms/InstructionList.hh"
#include "rms/InstructionCache.hh"
#include "rms/Buffer.hh"
#include "rms/Harvest.hh"
#include "rms/Stats.hh"
#include "daq/InterfaceDriverStats.hh"
#include "daq/InterfaceFirmwareStats.hh"

namespace RMS {

class Client : public DSM::Client {
public:
  Client(const char* partition, const char* interface, uint64_t timeout=100);
  Client(const char* partition, uint64_t timeout=100);
  Client(uint8_t     partition, const char* interface, uint64_t timeout=100);
  Client(uint8_t     partition, uint64_t timeout=100);
public:
  __attribute__((deprecated("use uint64_t timeout in milliseconds instead of unsigned 10ms tics")))
  Client(const char* partition, const char* interface, unsigned timeout);
  __attribute__((deprecated("use uint64_t timeout in milliseconds instead of unsigned 10ms tics")))
  Client(const char* partition, unsigned timeout);
  __attribute__((deprecated("use uint64_t timeout in milliseconds instead of unsigned 10ms tics")))
  Client(uint8_t     partition, const char* interface, unsigned timeout);
  __attribute__((deprecated("use uint64_t timeout in milliseconds instead of unsigned 10ms tics")))
  Client(uint8_t     partition, unsigned timeout);
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
  int load(const DAQ::LocationSet&, InstructionCache&, Harvest&);
public:
  bool reboot_rce(const DAQ::Location& server)     {return _reboot_rce(DAQ::LocationSet(server));}
  bool reboot_rce(const DAQ::LocationSet& servers) {return _reboot_rce(servers);}
  bool reboot_rce()                                {return _reboot_rce(_servers);}
public:
  DAQ::LocationSet reset(const DAQ::Location& server)     {return _reset(DAQ::LocationSet(server));}
  DAQ::LocationSet reset(const DAQ::LocationSet& servers) {return _reset(servers);}
  DAQ::LocationSet reset()                                {return _reset(_servers);}
public:
  DAQ::LocationSet reset_link(const DAQ::Location& server)     {return _reset_link(DAQ::LocationSet(server));}
  DAQ::LocationSet reset_link(const DAQ::LocationSet& servers) {return _reset_link(servers);}
  DAQ::LocationSet reset_link()                                {return _reset_link(_servers);}
public:
  bool stats(const DAQ::Location&, bool clear, RMS::Stats&,                  int32_t& error);
  bool stats(const DAQ::Location&, bool clear, DAQ::InterfaceDriverStats&,   int32_t& error);
  bool stats(const DAQ::Location&, bool clear, DAQ::InterfaceFirmwareStats&, int32_t& error);
public:
  const DAQ::LocationSet& sources() const {return (const DAQ::LocationSet&)_servers;}
public:
  const char* encode(uint8_t index, char buffer[]);
private:
  void             _access(     const DSI::Set&, const InstructionList&, Harvest&);
  bool             _reboot_rce( const DSI::Set&);
  DAQ::LocationSet _reset(      const DSI::Set&);
  DAQ::LocationSet _reset_link( const DSI::Set&);
private:
  Payload::Buffer _buffer;
  IPV4::Address   _lut[DSI::Set::SIZE];
};

}

#endif
