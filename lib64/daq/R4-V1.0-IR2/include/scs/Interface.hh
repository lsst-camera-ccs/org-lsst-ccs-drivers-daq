
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
 
#ifndef SCS_INTERFACE
#define SCS_INTERFACE

#include "dvi/Version.hh"
#include "osa/TimeStamp.hh"
#include "net/ipv4/Address.hh"
#include "dsi/Set.hh"
#include "dsi/Location.hh"
#include "dsi/Client.hh"

namespace SCS {

class Stats;

class Interface {
public:
  Interface(const char* partition, const char* interface);      
  Interface(unsigned    partition, const char* interface);      
public: 
  Interface(Interface&) = delete;
public:
 ~Interface() {}
public:
  const DSI::Set& servers() const {return _client.servers();} 
public:
  bool probe(uint8_t slot, DSI::Location& location, IPV4::Address&, DVI::Version&);
public:
  bool invalid(int32_t opcode) const;
public: 
  bool trigger(int32_t opcode, OSA::TimeStamp&);
public:
  bool stats(bool clear, Stats&, int32_t& error);
public:
  void restart();
  void shutdown();
private:
  bool _probe();
private:
  DSI::Client   _client;
  IPV4::Address _sequencer;
  DSI::Set      _sequencer_only;
  unsigned      _partition;
};
                 
}

#endif

