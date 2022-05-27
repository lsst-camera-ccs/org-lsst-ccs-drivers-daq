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
 
#ifndef SATA_GENERAL
#define SATA_GENERAL

#include "sata/Fis.hh"
#include "sata/Log.hh"
#include "sata/Fis.hh"

namespace SATA {

class General : public Log {
public: 
  General() : Log(FIS::LogAddress::DEVICE_STATISTICS, FIS::LogPage::GENERAL) {} 
public:
  General(General&) = delete;  
public:
 ~General() {}
public:
  uint64_t header()          const {return _statistic( 0/sizeof(uint64_t));}
  uint64_t lifetime_POR()    const {return _statistic( 8/sizeof(uint64_t));}
  uint64_t hours_POR()       const {return _statistic(16/sizeof(uint64_t));}
  uint64_t sectors_written() const {return _statistic(24/sizeof(uint64_t));}
  uint64_t writes()          const {return _statistic(32/sizeof(uint64_t));}
  uint64_t sectors_read()    const {return _statistic(40/sizeof(uint64_t));}
  uint64_t reads()           const {return _statistic(48/sizeof(uint64_t));}
  uint64_t timestamp()       const {return _statistic(56/sizeof(uint64_t));}
  uint64_t pending_errors()  const {return _statistic(64/sizeof(uint64_t));}
  uint64_t workload()        const {return _statistic(72/sizeof(uint64_t));}
  uint64_t usage()           const {return _statistic(80/sizeof(uint64_t));}
  uint64_t availability()    const {return _statistic(88/sizeof(uint64_t));}
  uint64_t resources_used()  const {return _statistic(96/sizeof(uint64_t));}
public:
  void dump(int indent=0) const;
  };

}

#endif 
