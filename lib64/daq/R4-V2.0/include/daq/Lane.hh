 
#ifndef DAQ_LANE
#define DAQ_LANE

#include <stdint.h>

namespace DAQ {

class Lane {
public:
  enum Type : uint8_t  {A=0, B=1, TEST=2, EMULATION=3, MAX=4, UNDEFINED=0xff};
public:
 static const char* encode(enum Type);  
};

}

#endif

