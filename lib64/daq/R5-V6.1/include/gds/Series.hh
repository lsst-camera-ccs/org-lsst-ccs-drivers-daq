#ifndef GDS_SERIES
#define GDS_SERIES

#include <stdint.h>

#include "osa/TimeStamp.hh"
#include "gds/LocationSet.hh"

namespace GDS {

class __attribute__((__packed__)) Series {
public:
  Series();
  Series(const OSA::TimeStamp& begin, uint16_t sequence, uint16_t stamps, const LocationSet& configured, const LocationSet& remaining);
  Series(const Series&);
public:
  ~Series() {};
public:
  Series& operator=(const Series&);
public:
  operator bool() const {return 0!=(uint64_t)_begin;}
public:
  const OSA::TimeStamp& begin()      const {return _begin;}
  uint16_t              sequence()   const {return _sequence;}
  uint16_t              stamps()     const {return _stamps;}
  const LocationSet&    configured() const {return _configured;}
  const LocationSet&    remaining()  const {return _remaining;}
public:
  void dump(unsigned indent=0) const;
protected:
  OSA::TimeStamp _begin;
  uint16_t       _sequence;
  uint16_t       _stamps;
  uint32_t       _reserved;
  LocationSet    _configured;
  LocationSet    _remaining;
};

}

#endif
