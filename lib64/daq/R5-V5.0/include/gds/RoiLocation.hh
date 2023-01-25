
#ifndef GDS_ROILOCATION
#define GDS_ROILOCATION

#include "gds/Location.hh"

namespace GDS {

class __attribute__((__packed__)) RoiLocation
{
public:
  RoiLocation() : _location(), _segment(0), _startrow(0), _startcol(0) {};
  RoiLocation(const Location&, uint16_t segment, uint16_t startrow, uint16_t startcol);
  RoiLocation(const RoiLocation&);
public:
  ~RoiLocation() {}
public:
  const char* encode(char* buffer) const;
  const char* encode() const;
  void dump(unsigned indent=0) const;
public:
  RoiLocation& operator=(const RoiLocation&);
public:
  const Location& location() const {return _location;}
  uint16_t        segment()  const {return _segment;}
  uint16_t        startrow() const {return _startrow;}
  uint16_t        startcol() const {return _startcol;}
private:
  Location _location;
  uint8_t  _reserved;
  uint16_t _segment;
  uint16_t _startrow;
  uint16_t _startcol;
}; 

  static_assert(!(sizeof(RoiLocation)&7), "Class GDS::RoiLocatoin is not an integral number of quadwords");

}



#endif

