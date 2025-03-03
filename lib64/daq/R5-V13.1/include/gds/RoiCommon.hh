
#ifndef GDS_ROICOMMON
#define GDS_ROICOMMON

#include <stdint.h>

namespace GDS {

class __attribute__((__packed__)) RoiCommon
{
public:
  RoiCommon();
  RoiCommon(uint16_t nrows, uint16_t ncols, uint16_t integration);
  RoiCommon(uint16_t nrows, uint16_t ncols, uint16_t integration, uint16_t overRows, uint16_t underCols, uint16_t overCols, uint16_t flushCount);
public:
  ~RoiCommon() {}
public:
  uint16_t nrows()       const {return _nrows;}
  uint16_t ncols()       const {return _ncols;}
  uint16_t integration() const {return _integration;}
  uint16_t underCols()   const {return _underCols;}
  uint16_t overCols()    const {return _overCols;}
  uint16_t overRows()    const {return _overRows;}
  uint16_t flushCount()  const {return _flushCount;}
public:
  unsigned pixels()      const {return _nrows*_ncols;}
  unsigned raw_pixels()  const {return (_nrows+_overRows)*(_ncols+_underCols+_overCols);}
public:
  void dump(unsigned indent=0) const;
private:
  friend class Parameters;
private:
  uint16_t _nrows;
  uint16_t _ncols;
  uint16_t _integration;
  uint16_t _overRows;
  uint16_t _underCols;
  uint16_t _overCols;
  uint16_t _flushCount;
  uint16_t _reserved;
};

  static_assert(!(sizeof(RoiCommon)&7), "Class GDS::RoiCommon is not an integral number of quadwords");

}

#endif

