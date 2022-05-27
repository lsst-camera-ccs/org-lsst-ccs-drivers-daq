#ifndef GDS_STATE
#define GDS_STATE

#include <stdint.h>

namespace GDS {

class __attribute__((__packed__)) State 
{
public:
  enum : uint8_t {UNDEFINED=0, STOPPED=1, PAUSED=2, RUNNING=3};
public:
  State() : _state(UNDEFINED) {};
  State(uint8_t state) : _state(state) {};
  State(const State& clone) : _state(clone._state) {};
public:
  State& operator=(const State& clone) {_state=clone._state; return *this;}
public:
  operator uint8_t() const {return _state;}
//public:
//  bool operator==(const State& other) {return _state==other._state;}
//  bool operator!=(const State& other) {return _state!=other._state;}
public:
  ~State() {};
public:
  const char* decode();
private:
  uint8_t _state;
};

}

#endif
