
#ifndef GDS_ROICOMMON
#define GDS_ROICOMMON

#include <stdint.h>

namespace GDS {

class __attribute__((__packed__)) RoiCommon
{
public:
  RoiCommon(uint16_t nrows=50, uint16_t ncols=50, uint16_t integration=50, uint16_t binning=1);
public:
  ~RoiCommon() {}
public:
  uint16_t nrows()       const {return _nrows;}
  uint16_t ncols()       const {return _ncols;}
  uint16_t integration() const {return _integration;}
  uint16_t binning()     const {return _binning;}
public:
  void dump(unsigned indent=0) const;
private:
  friend class Parameters;
private:
  uint16_t _nrows;
  uint16_t _ncols;
  uint16_t _integration;
  uint16_t _binning;
};

  static_assert(!(sizeof(RoiCommon)&7), "Class GDS::RoiCommon is not an integral number of quadwords");

}

#endif

