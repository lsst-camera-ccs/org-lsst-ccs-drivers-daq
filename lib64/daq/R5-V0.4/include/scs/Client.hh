
#ifndef SCS_CLIENT
#define SCS_CLIENT

#include "osa/TimeStamp.hh"
#include "net/ipv4/Address.hh"
#include "dsm/Client.hh"
#include "scs/Stats.hh"

namespace SCS {

class Client : public DSM::Client {
  enum : uint8_t {SEQUENCER_SLOT=1};
public:
  Client(const char* partition);
  Client(uint8_t     partition);
public:
  Client(const char* partition, const char* interface);
  Client(uint8_t     partition, const char* interface);
public:
  Client()        = delete;
  Client(Client&) = delete;
public:
  ~Client() {}
public:
  bool invalid(uint8_t opcode) const;
  bool trigger(uint8_t opcode, OSA::TimeStamp&);
  bool stats(uint8_t slot, Stats*, bool clear=false);
public:
  const DSI::Set& slots() const;
public:  
  const char* encode(uint8_t index, char buffer[]);
private:
  void _set_sequencer();
  uint8_t _lookup(const char* partition);
private:
  uint8_t       _pid;
  IPV4::Address _sequencer;
};

}

#endif

