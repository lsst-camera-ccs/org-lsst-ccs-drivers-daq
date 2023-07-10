/*
** ++
**  Package: SMI
**	
**
**  Abstract: LSST Storage Manager Interface
**
**  Implementation of LSST Storage Manager Interface
**
**  Author:
**      S. Maldonado, SLAC (smaldona@slac.stanford.edu)
**
**  Creation Date:
**	000 - November 04, 2018
**
**  Revision History:
**	None.
**
** --
*/
 
 
#ifndef SATA_STANDBY
#define SATA_STANDBY

#include "sata/Timer.hh"

namespace SATA {

class Standby : public Timer {
public:  
  Standby(uint8_t         value=Timer::DISABLE) : Timer(value) {} // one tic =  5 seconds, maximum value = 0xF0 (20 minutes) 
  Standby(uint16_t        value=Timer::DISABLE) : Timer(value) {} // one tic = 30 minutes, maximum value = 0xFB (330 minutes)     
  Standby(Timer::Reserved value=Timer::DISABLE) : Timer(value) {}
  Standby(const Standby& clone)                 : Timer(clone) {} 
public:
 ~Standby() {}  
  };
}

#endif
