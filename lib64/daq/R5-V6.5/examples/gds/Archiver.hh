#ifndef GDS_EXAMPLE_ARCHIVER
#define GDS_EXAMPLE_ARCHIVER

#include <stdio.h>

#include "gds/Decoder.hh"

#include "../RoiLimits.hh"

namespace GDS { namespace Example {

class Archiver : public GDS::Decoder
{
public:
  Archiver(const char* partition, const GDS::LocationSet& locs, const char* output_dir, bool raw=false);
public:
  ~Archiver();
private:
  void start    (const GDS::StateMetadata&, const GDS::SeriesMetadata&);
  void stop     (const GDS::StateMetadata&);
  void raw_stamp(const GDS::StateMetadata&, const GDS::RawStamp&);
  void stamp    (const GDS::StateMetadata&, const GDS::Stamp&);
private:
  uint8_t* allocate(unsigned size);
private:
  bool     _raw;
  char     _directory[256];
  FILE*    _dfile[GDS::Set::SIZE];
  FILE*    _rfile[GDS::Set::SIZE];
  unsigned _stamps[GDS::Set::SIZE];
  unsigned _rstamps[GDS::Set::SIZE];
  uint8_t  _stamp_buf[RoiLimits::MAX_ROWS*RoiLimits::MAX_COLS*sizeof(int32_t)];
};

}}
#endif
