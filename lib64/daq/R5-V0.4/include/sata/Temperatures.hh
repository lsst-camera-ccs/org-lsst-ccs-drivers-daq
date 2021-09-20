/*
** ++
**  Package: SMI
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
 
#ifndef SATA_TEMPERATURES
#define SATA_TEMPERATURES

#include "sata/Fis.hh"
#include "sata/Log.hh"
#include "sata/Fis.hh"

namespace SATA {

class Temperatures : public Log {
public: 
  Temperatures() : Log(FIS::LogAddress::DEVICE_STATISTICS, FIS::LogPage::TEMPERATURES) {}
public:
  Temperatures(Temperatures&) = delete;  
public:
 ~Temperatures() {}
public:
  uint64_t header()          const {return _statistic(  0/sizeof(uint64_t));}
  uint64_t current()         const {return _statistic(  8/sizeof(uint64_t));}
  uint64_t shortTerm()       const {return _statistic( 16/sizeof(uint64_t));}
  uint64_t longTerm()        const {return _statistic( 24/sizeof(uint64_t));}
  uint64_t high()            const {return _statistic( 32/sizeof(uint64_t));}
  uint64_t low()             const {return _statistic( 40/sizeof(uint64_t));}
  uint64_t shortTerm_high()  const {return _statistic( 48/sizeof(uint64_t));}
  uint64_t shortTerm_low()   const {return _statistic( 56/sizeof(uint64_t));}
  uint64_t longTerm_high()   const {return _statistic( 64/sizeof(uint64_t));}
  uint64_t longTerm_low()    const {return _statistic( 72/sizeof(uint64_t));} 
  uint64_t time_over_max()   const {return _statistic( 80/sizeof(uint64_t));}
  uint64_t max()             const {return _statistic( 88/sizeof(uint64_t));}
  uint64_t time_under_min()  const {return _statistic( 96/sizeof(uint64_t));}
  uint64_t min()             const {return _statistic(104/sizeof(uint64_t));}
public:
  void dump(int indent=0) const;
  };

}

#endif 
