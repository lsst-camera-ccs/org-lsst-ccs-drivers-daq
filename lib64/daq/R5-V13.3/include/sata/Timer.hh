/*
** ++
**  Package: SATA
**	
**
**  Abstract: LSST Storage Sector Interface
**
**  Implementation of LSST Storage Sector Interface
**
**  Author:
**      S. Maldonado, SLAC (smaldona@slac.stanford.edu)
**
**  Creation Date:
**	000 - Sept 15, 2018
**
**  Revision History:
**	None.
**
** --
*/
 
#ifndef SATA_TIMER
#define SATA_TIMER

#include <stdint.h>

namespace SATA {

class Timer {
public:
  enum Reserved : uint16_t {DISABLE = 0, TWENTY_ONE_MINUTES = 0xFC, EIGHT_TO_TWELVE_HOURS = 0xFD, TWENTY_ONE_MINUTES_FIFTEEN_SECONDS = 0xFF};
public:
  Timer(uint8_t       value=DISABLE) : _value(value)        {} // one tic =  5 seconds, maximum value = 0xF0 (20 minutes) 
  Timer(uint16_t      value=DISABLE) : _value(value + 0xF0) {} // one tic = 30 minutes, maximum value = 0xFB (330 minutes)     
  Timer(enum Reserved value=DISABLE) : _value(value)        {}
  Timer(const Timer& clone)          : _value(clone._value) {} 
public:  
  Timer() = delete; 
public:
  ~Timer() {}
public:
  uint16_t value() const {return _value;} 
private:
  uint16_t _value;
  };

}

#endif
