#ifndef REB_REB_IMAGE_STATS
#define REB_REB_IMAGE_STATS

#include <stdint.h>

#include "rms/Client.hh"
#include "daq/LocationSet.hh"

namespace REB {

class RebImageStats {
public:
  enum {NUMOF_VCS=3};
public:
  RebImageStats(const char* partition);
  RebImageStats(const char* partition, const char* interface);
public:
 ~RebImageStats() {}
public:
  bool read(const DAQ::Location&);
public:
  uint32_t sent(int vc)      const {return _sent[vc];}
  uint32_t discarded(int vc) const {return _discarded[vc];}
  uint32_t truncated(int vc) const {return _truncated[vc];}
  uint32_t format(int vc)    const {return _format[vc];}
public:
  void dump(const char* prefix, bool header=false) const;
  void dump_full(const char* prefix) const;
public:
  const DAQ::LocationSet& locations() const {return _rms.sources();}
private:
  RMS::Client _rms;
  uint32_t    _sent[NUMOF_VCS];
  uint32_t    _discarded[NUMOF_VCS];
  uint32_t    _truncated[NUMOF_VCS];
  uint32_t    _format[NUMOF_VCS];
};

}

#endif
