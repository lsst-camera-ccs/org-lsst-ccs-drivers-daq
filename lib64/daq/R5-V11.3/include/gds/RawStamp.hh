
#ifndef GDS_RAWSTAMP
#define GDS_RAWSTAMP

#include <stddef.h>
#include <inttypes.h>

#include "daq/Sensor.hh"

namespace GDS {

class Stamp;
class SeriesMetadata;

class __attribute__ ((__packed__)) RawStamp
{
public:
  static unsigned calc_size(const GDS::SeriesMetadata&);
  static unsigned calc_npix(const GDS::SeriesMetadata&);
public:
  RawStamp(uint8_t* content, uint32_t size) : _content(content), _size(size) {};
  RawStamp() : _content(NULL), _size(0) {};
  RawStamp(const RawStamp& clone) : _content(clone._content), _size(clone._size) {};
public:
  ~RawStamp() {};
public:
  RawStamp& operator=(const RawStamp& clone) {_content=clone._content; _size=clone._size; return *this;}
public:
  const uint8_t* content() const {return _content;}
  const uint32_t size()    const {return _size;}
  const unsigned npix()    const {return _size/(int)DAQ::Sensor::Segment::SIZEOF;}
public:
  void decode(Stamp&, const SeriesMetadata&, bool forceBE=false) const;
  void unpack(int32_t* pixels,               bool forceBE=false) const;
private:
  uint8_t* _content;
  uint32_t _size;
};
}

#endif
