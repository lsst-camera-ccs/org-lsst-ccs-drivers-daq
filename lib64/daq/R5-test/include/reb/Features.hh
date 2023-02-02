#ifndef REB_FEATURES
#define REB_FEATURES

#include <stdint.h>

#include "daq/Sensor.hh"
#include "daq/Location.hh"

namespace REB {

class Features {
public:
  enum : uint32_t {
    NONE=0,
    TYPE      = 1<<0,
    MULTIBOOT = 1<<1,
    MULTISEQ  = 1<<2,
    STOPCMD   = 1<<3,
    IDLELOOP  = 1<<4};
public:
  static bool valid(const DAQ::Location&,    uint32_t version, uint32_t include, uint32_t exclude=NONE);
  static bool valid(const DAQ::Sensor::Type, uint32_t version, uint32_t include, uint32_t exclude=NONE);
};
}
#endif
