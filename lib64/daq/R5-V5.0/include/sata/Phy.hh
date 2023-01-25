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
 
#ifndef SATA_PHY
#define SATA_PHY

#include "sata/Fis.hh"
#include "sata/Log.hh"

namespace SATA {

class Phy : public Log {
public: 
  Phy() : Log(FIS::LogAddress::SATA_EVENTS, 0) {} 
public:
  Phy(Phy&) = delete;  
public:
 ~Phy() {}
public:
  void dump(int indent=0) const;
public:
};
  
}

#endif 
