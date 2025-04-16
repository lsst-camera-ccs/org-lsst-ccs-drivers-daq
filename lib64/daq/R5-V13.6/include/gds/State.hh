#ifndef GDS_STATE
#define GDS_STATE

#include <stdint.h>
#include "osa/TimeStamp.hh"

namespace GDS {

class __attribute__((__packed__)) State 
{
public:
  enum : uint8_t {UNDEFINED=0, IDLECLEARING=1, PAUSED=2, RUNNING=3, ERROR=4, OFF=5, CLEARING=6, IDLEPAUSE=7};
public:
  State() : _entered(false), _state(UNDEFINED) {};
  State(uint8_t state, const OSA::TimeStamp& time) : _entered(time), _state(state) {};
  State(const State& clone) : _entered(clone._entered), _state(clone._state) {};
public:
  State& operator=(const State& clone) {_entered=clone._entered; _state=clone._state; return *this;}
public:
  operator uint8_t() const {return _state;}
public:
  const OSA::TimeStamp& entered() const {return _entered;}
public:
  ~State() {};
public:
  const char* decode() const;
  const char* dump(unsigned indent=0) const;
private:
  OSA::TimeStamp _entered;
  uint8_t        _state;
};

}

#endif
