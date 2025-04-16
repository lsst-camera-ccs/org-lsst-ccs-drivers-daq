
#ifndef REB_BITFILE_HH
#define REB_BITFILE_HH

#include "osa/File.hh"

namespace REB {

class Bitfile : public OSA::File
{
public:
  Bitfile(const char* filename);
public:
  ~Bitfile();
public:
  const uint32_t* bitstream(unsigned& length) const {length = _bitstreamlen; return (uint32_t*)_bitstream;}
public:
  unsigned    filelen()      const {return _filelen;}
  unsigned    headerlen()    const {return _headerlen;}
  unsigned    bitstreamlen() const {return _bitstreamlen;}
  const char* design()       const {return _design;}
  uint32_t    version()      const {return _version;}
  const char* device()       const {return _device;}
  const char* date()         const {return _date;}
  const char* time()         const {return _time;}
public:
  void dump() const;
private:
  void _parse_header();
private:
  unsigned _filelen;
  unsigned _headerlen;
  unsigned _bitstreamlen;
  char*    _design;
  uint32_t _version;
  char*    _device;
  char*    _date;
  char*    _time;
  uint8_t* _bitstream;
  uint8_t* _buf8;
};
}
#endif
