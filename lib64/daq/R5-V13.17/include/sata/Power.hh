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
 
#ifndef SATA_POWER
#define SATA_POWER

#include "sata/Fis.hh"

namespace SATA {

class Power {
public:
  enum State : uint8_t {STANDBY=0x0, STANDBY_EPC=0x1, IDLE=0x80, IDLE_EPC_A=0x81, IDLE_EPC_B=0x82, IDLE_EPC_C=0x83, IDLE_ACTIVE=0xFF};
public:
  Power()            { _d2h.sector_count = STANDBY;} 
  Power(State state) { _d2h.sector_count = state;} 
public:   
 ~Power() {}
public:
  State       state() const {return (State)_d2h.sector_count;}
  const char* name()  const;  
public:
  void dump(const char* title, int indent=0) const;
public:
  FIS::__D2H* d2h() {return &_d2h;}
private:
  FIS::__D2H _d2h;  
  };

}

#endif 
