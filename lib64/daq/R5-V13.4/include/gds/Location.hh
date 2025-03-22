
#ifndef GDS_LOCATION
#define GDS_LOCATION

#include "daq/Location.hh"

namespace GDS {

class __attribute__((__packed__)) Location {
public:
  enum : uint8_t {MAX_BAYS=25, MAX_BOARDS=3, MAX_SENSORS=3};
  enum : uint8_t {UNDEFINED=0xFF};
public:
  Location() : _location(UNDEFINED) {};
  Location(uint8_t location) : _location(location) {};
  Location(uint8_t bay, uint8_t board, uint8_t sensor);
  Location(const DAQ::Location& location, uint8_t sensor);
  Location(const char*);
  Location(const Location& clone);
public:
  ~Location() {};
public:
  const DAQ::Location source() const;
  uint8_t             bay()    const;
  uint8_t             board()  const;
  uint8_t             sensor() const;
public:
  Location& operator=(const Location& clone);
public:
  operator bool() const {return _location != UNDEFINED;}
public:
  uint8_t index() const {return _location;}
public:
  const char* encode(char* buffer) const;
  const char* encode() const;
private:
  uint8_t _location;
};

}
  
#endif
