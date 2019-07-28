#ifndef RMS_ERRORS
#define RMS_ERRORS

#include <stdint.h>

namespace RMS {

class Errors
{
public:
  static const char* decode(int32_t error);
public:
  enum : int32_t {UNKNOWN_LOCATION = -2,
		  COMMAND_TIMEOUT  = -1,
		  SUCCESS          =  0, 
		  INVALID_REQUEST  =  1, 
		  EMPTY_REQUEST    =  2, 
		  LINK_DOWN        =  3, 
		  NOT_YET_OPENED   =  4,
		  IN_RESET         =  5,
		  RESET_TIMEOUT    =  6,
		  INSUF_RESOURCES  =  7};
};
}

#endif

