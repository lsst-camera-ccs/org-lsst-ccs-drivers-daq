
#ifndef GDS_RAWSTAMP
#define GDS_RAWSTAMP

#include <inttypes.h>

namespace GDS {

class Stamp;
class SeriesMetadata;

class __attribute__ ((__packed__)) RawStamp
{
public:
  RawStamp(uint8_t* content, uint32_t size) : _content(content), _size(size) {};
  RawStamp() = delete;
  RawStamp(const RawStamp& clone) : _content(clone._content), _size(clone._size) {};
public:
  ~RawStamp() {};
public:
  RawStamp& operator=(const RawStamp& clone) {_content=clone._content; _size=clone._size; return *this;}
public:
  const uint8_t* content() const {return _content;}
  const uint32_t size()    const {return _size;}
public:
  void decode(Stamp&, const SeriesMetadata&) const;
private:
  uint8_t* _content;
  uint32_t _size;
};
}

#endif
