#ifndef SATA_TRANSPORT
#define SATA_TRANSPORT

#include "sata/Fis.hh"
#include "sata/Log.hh"

namespace SATA {

class Transport : public Log {
public:
  Transport() : Log(FIS::LogAddress::DEVICE_STATISTICS, FIS::LogPage::TRANSPORT) {}
public:
  Transport(Transport&) = delete;
public:
 ~Transport() {}
public:
  uint64_t header()        const {return _statistic( 0/sizeof(uint64_t));}
  uint64_t hw_resets()     const {return _statistic( 8/sizeof(uint64_t));}
  uint64_t asr_events()    const {return _statistic(16/sizeof(uint64_t));}
  uint64_t interface_crc() const {return _statistic(24/sizeof(uint64_t));}
public:
  void dump(int indent=0) const;
  };

}

#endif
