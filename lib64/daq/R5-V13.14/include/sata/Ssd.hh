#ifndef SATA_SSD
#define SATA_SSD

#include "sata/Fis.hh"
#include "sata/Log.hh"

namespace SATA {

class Ssd : public Log {
public:
  Ssd() : Log(FIS::LogAddress::DEVICE_STATISTICS, FIS::LogPage::SSD) {}
public:
  Ssd(Ssd&) = delete;
public:
 ~Ssd() {}
public:
  uint64_t header()    const {return _statistic(0/sizeof(uint64_t));}
  uint64_t endurance() const {return _statistic(8/sizeof(uint64_t));}
public:
  void dump(int indent=0) const;
  };

}

#endif
