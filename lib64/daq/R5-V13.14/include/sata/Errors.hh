#ifndef SATA_ERRORS
#define SATA_ERRORS

#include "sata/Fis.hh"
#include "sata/Log.hh"

namespace SATA {

class Errors : public Log {
public:
  Errors() : Log(FIS::LogAddress::DEVICE_STATISTICS, FIS::LogPage::ERRORS) {}
public:
  Errors(Errors&) = delete;
public:
 ~Errors() {}
public:
  uint64_t header()          const {return _statistic( 0/sizeof(uint64_t));}
  uint64_t interface_crc()   const {return _statistic( 8/sizeof(uint64_t));}
  uint64_t uncorrectable()   const {return _statistic(16/sizeof(uint64_t));}
  uint64_t resets()          const {return _statistic(24/sizeof(uint64_t));}
  uint64_t mech_failures()   const {return _statistic(32/sizeof(uint64_t));}
  uint64_t pending_recover() const {return _statistic(40/sizeof(uint64_t));}
public:
  void dump(int indent=0) const;
  };

}

#endif
