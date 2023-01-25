#ifndef IMS_EDITOR_CRC32
#define IMS_EDITOR_CRC32

#include "ims/Decoder.hh"

namespace IMS {namespace Editor {

class Crc32 : public IMS::Decoder {
public:
  Crc32(IMS::Image& image);
  Crc32(IMS::Image& image, const DAQ::LocationSet& filter);
public:
  ~Crc32();
public:
  void process(IMS::Science::Source&   source, uint64_t length, uint64_t offset);
  void process(IMS::Guiding::Source&   source, uint64_t length, uint64_t offset);
  void process(IMS::Wavefront::Source& source, uint64_t length, uint64_t offset);
public:
  uint32_t  crc(const DAQ::Location& loc) const {return  _crc[loc.index()];}
  uint64_t size(const DAQ::Location& loc) const {return _size[loc.index()];}
public:
  void dump(int indent=0) const;
  void dump(const DAQ::Location&, int indent=0) const;
private:
  void _calc(IMS::Source& source, uint64_t length, uint64_t base);
private:
  DAQ::LocationSet _locs;
  uint8_t*         _buffer;
  uint32_t         _crc [DAQ::LocationSet::SIZE];
  uint64_t         _size[DAQ::LocationSet::SIZE];
};

}}
#endif
