
/*
**  Package:
**	
**
**  Abstract:
**      
**
**  Author:
**      Michael Huffer, SLAC (mehsys@slac.stanford.edu)
**
**  Creation Date:
**	    000 - April 06, 2011
**
**  Revision History:
**	    None.
**
** --
*/
 
#ifndef DAQ_SENSOR
#define DAQ_SENSOR

#include <stdint.h>

namespace DAQ {

class Sensor {
public:
  enum Type          : uint8_t  {SCIENCE=3, GUIDE=2, WAVEFRONT=1, UNDEFINED=0, NUMOF=3};
  enum class Pixel   : unsigned {SIZEOF=18};                                                                // in units of bits    
  enum class Segment : unsigned {NUMOF=16, MIN=0, MAX=NUMOF-1, SIZEOF=(NUMOF * (unsigned)Pixel::SIZEOF)/8}; // per CCD (in units of bytes)
  enum class Number  : unsigned {ZERO=0, ONE=1, TWO=2}; 
public:
 static const char* encode(enum Type);  
};

}

#endif

