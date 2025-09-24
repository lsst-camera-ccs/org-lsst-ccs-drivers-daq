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
 
 
#ifndef SATA_IDLE
#define SATA_IDLE

#include "sata/Timer.hh"

namespace SATA {

class Idle : public Timer {
public:  
  Idle(uint8_t         value=Timer::DISABLE) : Timer(value) {} // one tic =  5 seconds, maximum value = 0xF0 (20 minutes) 
  Idle(uint16_t        value=Timer::DISABLE) : Timer(value) {} // one tic = 30 minutes, maximum value = 0xFB (330 minutes)     
  Idle(Timer::Reserved value=Timer::DISABLE) : Timer(value) {}
  Idle(const Idle& clone)                    : Timer(clone) {} 
public:
 ~Idle() {}  
  };
}

#endif
