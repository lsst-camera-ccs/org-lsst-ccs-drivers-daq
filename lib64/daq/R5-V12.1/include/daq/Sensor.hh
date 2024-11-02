 
#ifndef DAQ_SENSOR
#define DAQ_SENSOR

#include <stdint.h>

namespace DAQ {

class Sensor {
public:
  enum Type          : uint8_t  {SCIENCE=3, GUIDE=2, WAVEFRONT=1, UNDEFINED=0, NUMOF=3};
  enum Vendor        : uint8_t  {ITL=0, E2V=1, UNDEF=2};
  enum class Pixel   : unsigned {SIZEOF=18};                                                                // in units of bits    
  enum class Segment : unsigned {NUMOF=16, MIN=0, MAX=NUMOF-1, SIZEOF=(NUMOF * (unsigned)Pixel::SIZEOF)/8}; // per CCD (in units of bytes)
  enum class Number  : unsigned {ZERO=0, ONE=1, TWO=2}; 
public:
 static const char* encode(enum Type);
 static const char* encode(enum Vendor);
 static enum Vendor vendor(const char*);
};

}

#endif
