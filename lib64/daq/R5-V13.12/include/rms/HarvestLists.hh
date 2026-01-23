#ifndef RMS_HARVEST_LISTS
#define RMS_HARVEST_LISTS

#include "rms/Harvest.hh"

#include "daq/Location.hh"
#include "daq/LocationSet.hh"
#include "rms/InstructionList.hh"

namespace RMS {

class HarvestLists : public Harvest {
public:
  HarvestLists();
public:
  HarvestLists(const HarvestLists&) = delete;
public:
  virtual ~HarvestLists() {}
public:
  void process(const DAQ::Location&, const InstructionList&, int32_t error);
public:
  void summary() const;
public:
  const DAQ::LocationSet& sources()                          const {return _sources;}
  int32_t                 errors(const DAQ::Location& loc)   const {return _errors[loc.index()];}
  const InstructionList&  response(const DAQ::Location& loc) const {return _responses[loc.index()];}
public:
  void clear();
private:
  DAQ::LocationSet _sources;
  int32_t          _errors[DAQ::LocationSet::SIZE];
  InstructionList  _responses[DAQ::LocationSet::SIZE];
};

}

#endif
