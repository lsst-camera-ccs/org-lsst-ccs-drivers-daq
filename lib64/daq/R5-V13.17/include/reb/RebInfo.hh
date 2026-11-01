#ifndef REB_REB_INFO
#define REB_REB_INFO

#include "daq/FpgaBuildInfo.hh"
#include "daq/LocationSet.hh"
#include "rms/Client.hh"

namespace REB {

class RebInfo : public FpgaBuildInfo {
public:
  RebInfo(const char* partition);
  RebInfo(const char* partition, const char* interface);
public:
 ~RebInfo() {}
public:
  bool read(const DAQ::Location&);
public:
  uint32_t clockPeriod() const {return _clockPeriod;}
  uint64_t boardId()     const {return _boardId;}
  const DAQ::LocationSet& validLocations() const {return _validLocs;}
public:
  void dump(const char* prefix, unsigned flags=DUMP_ALL, bool verbose=false) const;
public:
  const DAQ::LocationSet& locations() const {return _rms.sources();}
public:
  // Version register field accessors
  static uint32_t    boardType(uint32_t v)   {return (v >> 28) & 0xF;}
  static uint32_t    multiboot(uint32_t v)   {return (v >> 24) & 0x1;}
  static uint32_t    sciVersion(uint32_t v)  {return (v >> 16) & 0xFF;}
  static uint32_t    hwRevision(uint32_t v)  {return (v >> 12) & 0xF;}
  static uint32_t    multiSeq(uint32_t v)    {return (v >>  8) & 0x1;}
  static uint32_t    fwRevision(uint32_t v)  {return (v >>  0) & 0xFF;}
  static const char* boardName(uint32_t v);
private:
  RMS::Client      _rms;
  uint64_t         _boardId;
  uint32_t         _clockPeriod;
  DAQ::LocationSet _validLocs;
  bool             _boardIdValid;
  bool             _locsValid;
};

}

#endif
