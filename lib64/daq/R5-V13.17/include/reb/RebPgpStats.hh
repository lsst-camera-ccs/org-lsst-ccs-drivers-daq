#ifndef REB_REB_PGP_STATS
#define REB_REB_PGP_STATS

#include <stdint.h>

#include "rms/Client.hh"
#include "daq/LocationSet.hh"

namespace REB {

class RebPgpStats {
public:
  enum {NUMOF_VCS=4};
public:
  RebPgpStats(const char* partition);
  RebPgpStats(const char* partition, const char* interface);
public:
 ~RebPgpStats() {}
public:
  bool read(const DAQ::Location&);
  void clear(const DAQ::Location&);
public:
  uint32_t linkStatus()            const {return _linkStatus;}
  uint32_t remLinkData()           const {return _remLinkData;}
  uint32_t cellErrors()            const {return _cellErrors;}
  uint32_t linkDown()              const {return _linkDown;}
  uint32_t linkErrors()            const {return _linkErrors;}
  uint32_t remOverflow(int vc)     const {return _remOverflow[vc];}
  uint32_t rxErrors()              const {return _rxErrors;}
  uint32_t rxFrames()              const {return _rxFrames;}
  uint32_t locOverflow(int vc)     const {return _locOverflow[vc];}
  uint32_t txErrors()              const {return _txErrors;}
  uint32_t txFrames()              const {return _txFrames;}
public:
  void dump(const char* prefix, bool header=false) const;
  void dump_full(const char* prefix) const;
public:
  const DAQ::LocationSet& locations() const {return _rms.sources();}
public:
  // Link status bit field accessors
  static bool phyRxReady(uint32_t s)    {return (s >>  0) & 1;}
  static bool phyTxReady(uint32_t s)    {return (s >>  1) & 1;}
  static bool rxLinkReady(uint32_t s)   {return (s >>  2) & 1;}
  static bool remLinkReady(uint32_t s)  {return (s >>  3) & 1;}
  static bool txLinkReady(uint32_t s)   {return (s >>  4) & 1;}
public:
  uint32_t opcodeCount()  const {return _opcodeCount;}
  uint32_t opcodeLast()   const {return _opcodeLast;}
  uint32_t noticeCount()  const {return _noticeCount;}
  uint32_t noticeLast0()  const {return _noticeLast0;}
  uint32_t noticeLast1()  const {return _noticeLast1;}
  uint32_t pgpSide()      const {return _pgpSide;}
private:
  RMS::Client _rms;
  uint32_t    _linkStatus;
  uint32_t    _remLinkData;
  uint32_t    _cellErrors;
  uint32_t    _linkDown;
  uint32_t    _linkErrors;
  uint32_t    _remOverflow[NUMOF_VCS];
  uint32_t    _rxErrors;
  uint32_t    _rxFrames;
  uint32_t    _locOverflow[NUMOF_VCS];
  uint32_t    _txErrors;
  uint32_t    _txFrames;
  uint32_t    _opcodeCount;
  uint32_t    _opcodeLast;
  uint32_t    _noticeCount;
  uint32_t    _noticeLast0;
  uint32_t    _noticeLast1;
  uint32_t    _pgpSide;
};

}

#endif
