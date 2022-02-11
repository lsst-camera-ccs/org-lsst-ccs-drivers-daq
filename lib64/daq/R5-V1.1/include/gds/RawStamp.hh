
#ifndef GDS_RAWSTAMP
#define GDS_RAWSTAMP

namespace GDS {

class __attribute__ ((__packed__)) RawStamp
{
public:
  RawStamp(uint8_t* content, uint32_t size) : _content(content), _size(size) {};
  RawStamp() = delete;
  RawStamp(const RawStamp&) = delete;
public:
  ~RawStamp() {};
public:
  uint8_t* content() {return _content;}
  uint32_t size() const {return _size;}
private:
  uint8_t* _content;
  uint32_t _size;
};
}

#endif
