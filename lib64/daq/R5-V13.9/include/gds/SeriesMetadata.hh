#ifndef GDS_SERIESMETADATA
#define GDS_SERIESMETADATA

#include <stdint.h>

#include "dvi/Version.hh"
#include "osa/TimeStamp.hh"
#include "daq/Sensor.hh"
#include "gds/RoiCommon.hh"
#include "gds/RoiLocation.hh"

namespace GDS {
  
class __attribute__((__packed__)) SeriesMetadata
{
public:
  SeriesMetadata(const RoiCommon&, const RoiLocation&, bool splitroi, const DVI::Version&, uint32_t firmware, uint64_t serial_number, DAQ::Sensor::Vendor, const char* platform, const char* id);
  SeriesMetadata(const SeriesMetadata&);
  SeriesMetadata(const SeriesMetadata*);
  SeriesMetadata() : _valid(false) {};
public:
  ~SeriesMetadata() {};
public:
  SeriesMetadata& operator=(const SeriesMetadata&);
public:
  operator bool() const {return _valid;}
public:
  const RoiCommon&      common()        const {return _common;}
  const RoiLocation&    location()      const {return _location;}
  const DVI::Version    software()      const {return _software;}
  uint32_t              firmware()      const {return _firmware;}
  uint64_t              serial_number() const {return _serial_number;}
  bool                  splitroi()      const {return _splitroi;}
  DAQ::Sensor::Vendor   vendor()        const {return _vendor;}
  const char*           platform()      const {return _platform;}
  const char*           id()            const {return _id;}
public:
  void dump(int indent=0) const;
private:
  RoiCommon           _common;
  RoiLocation         _location;
  DVI::Version        _software;
  uint32_t            _firmware;
  uint64_t            _serial_number;
  bool                _valid;
  bool                _splitroi;
  DAQ::Sensor::Vendor _vendor;
  char                _platform[32];
  char                _id[64];
};

}
#endif
