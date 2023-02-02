#ifndef GDS_STATUS
#define GDS_STATUS

#include "osa/TimeStamp.hh"
#include "gds/State.hh"
#include "gds/Exception.hh"

namespace GDS {

class __attribute__((__packed__)) Status
{
public:
  Status(const OSA::TimeStamp& timestamp, int32_t status, uint16_t sequence, State in, State out) : _timestamp(timestamp), _status(status), _sequence(sequence), _in(in), _out(out) {};
public:
  Status() : _timestamp(), _status(Exception::Id::INVALID), _sequence(0), _in(), _out() {};
public:
  ~Status() {};
public:
  const OSA::TimeStamp& timestamp() const {return _timestamp;}
  int32_t               status()    const {return _status;}
  uint16_t              sequence()  const {return _sequence;}
  const State&          state_in()  const {return _in;}
  const State&          state_out() const {return _out;}
public:
  operator bool() const {return _status != Exception::Id::INVALID;}
public:
  void dump();
public:
  OSA::TimeStamp _timestamp;
  int32_t        _status;
  uint16_t       _sequence;
  State          _in;
  State          _out;
};

static_assert(!(sizeof(Status)&7), "Class GDS::Status is not an integral number of quadwords");

}

#endif
