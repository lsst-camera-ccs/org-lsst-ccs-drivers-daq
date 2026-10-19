#ifndef IMS_EDITOR_FILE
#define IMS_EDITOR_FILE

#include "ims/Decoder.hh"

namespace IMS {namespace Editor {

class File : public IMS::Decoder {
public:
  File()            = delete;
  File(const File&) = delete;
public:
  File(IMS::Image& image, const char* directory);
  File(IMS::Image& image, const DAQ::LocationSet& filter, const char* directory);
public:
  ~File();
public:
  void process(IMS::Science::Source&   source, uint64_t length, uint64_t offset);
  void process(IMS::Guiding::Source&   source, uint64_t length, uint64_t offset);
  void process(IMS::Wavefront::Source& source, uint64_t length, uint64_t offset);
private:
  void _write(IMS::Source& source, uint64_t length, uint64_t base);
public:
  uint64_t processed() const {return _processed;}
private:
  uint64_t         _processed;
  DAQ::LocationSet _locs;
  uint8_t*         _buffer;
  char             _filebase[256];
  FILE*            _file[DAQ::LocationSet::SIZE];
};

}}
#endif
