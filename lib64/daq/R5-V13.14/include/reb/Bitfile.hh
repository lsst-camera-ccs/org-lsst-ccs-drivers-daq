
#ifndef REB_BITFILE_HH
#define REB_BITFILE_HH

#include "osa/File.hh"

#include "daq/LocationSet.hh"

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
  const char* userid()       const {return _userid;}
  const char* vivadover()    const {return _vivadover;}
  uint32_t    version()      const {return _version;}
  const char* device()       const {return _device;}
  const char* date()         const {return _date;}
  const char* time()         const {return _time;}
public:
  const DAQ::LocationSet& valid_locations() const {return _valid_locs;}
public:
  void dump() const;
private:
  void _parse_header();
  void _parse_design(char* design);
  void _set_locations();
private:
  unsigned         _filelen;
  unsigned         _headerlen;
  unsigned         _bitstreamlen;
  char*            _design;
  char*            _userid;
  char*            _vivadover;
  uint32_t         _version;
  char*            _device;
  char*            _date;
  char*            _time;
  uint8_t*         _bitstream;
  uint8_t*         _buf8;
  DAQ::LocationSet _valid_locs;
};
}
#endif
