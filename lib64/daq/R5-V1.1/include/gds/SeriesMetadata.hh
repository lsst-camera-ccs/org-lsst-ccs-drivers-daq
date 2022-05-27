#ifndef GDS_SERIESMETADATA
#define GDS_SERIESMETADATA

#include <stdint.h>

#include "dvi/Version.hh"
#include "osa/TimeStamp.hh"
#include "gds/RoiCommon.hh"
#include "gds/RoiLocation.hh"

namespace GDS {
  
class __attribute__((__packed__)) SeriesMetadata
{
public:
  SeriesMetadata(const RoiCommon&, const RoiLocation&, const DVI::Version&, uint32_t firmware, uint64_t serial_number);
  SeriesMetadata(const SeriesMetadata&);
  SeriesMetadata(const SeriesMetadata*);
  SeriesMetadata() {}; //= delete;
public:
  ~SeriesMetadata() {};
public:
  SeriesMetadata& operator=(const SeriesMetadata&);
public:
  const RoiCommon&      common()        const;
  const RoiLocation&    location()      const;
  const DVI::Version    software()      const;
  uint32_t              firmware()      const;
  uint64_t              serial_number() const;
public:
  void dump(int indent=0) const;
private:
  RoiCommon      _common;
  RoiLocation    _location;
  DVI::Version   _software;
  uint32_t       _firmware;
  uint64_t       _serial_number;
  uint32_t       _reserved;
};

}
#endif
