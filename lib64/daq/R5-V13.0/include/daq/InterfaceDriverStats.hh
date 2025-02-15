#ifndef DAQ_INTERFACEDRIVERSTATS
#define DAQ_INTERFACEDRIVERSTATS

#include <stdint.h>

namespace DAQ {

class InterfaceDriverStats
{
  friend class Interface;
public:
  enum {NUMOF_VCS=4};
public:
  InterfaceDriverStats() {};
  InterfaceDriverStats(const InterfaceDriverStats&);
public:
  ~InterfaceDriverStats() {};
public:
  InterfaceDriverStats& operator=(const InterfaceDriverStats&);
public:
  uint64_t received()              const {return _received;}
  uint64_t errors()                const {return _errors;}
  uint64_t rx(int vc)              const {return _rx[vc];}
public:
  void dump(const char* source, bool header = false) const;
private:
  uint64_t _received;
  uint64_t _errors;
  uint64_t _rx[NUMOF_VCS];
};
  
}


#endif
