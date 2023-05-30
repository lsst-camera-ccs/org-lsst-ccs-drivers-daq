
#ifndef RMS_HARVEST_STATUS
#define RMS_HARVEST_STATUS

#include "rms/Harvest.hh"

#include "daq/Location.hh"
#include "daq/LocationSet.hh"
#include "rms/InstructionList.hh"

namespace RMS {

class HarvestStatus : public Harvest {
public:
  HarvestStatus(bool verbose=false) : _verbose(verbose) {};
public:
  HarvestStatus(const HarvestStatus&) = delete;
public:
  virtual ~HarvestStatus() {} 
public:
  void process(const DAQ::Location&, const InstructionList&, int32_t error);
public:
  operator bool() const { return (_response_errors+_instruction_errors); }
public:
  void summary() const;
public:
  unsigned sources()            const {return _sources.numof();}
  unsigned responses()          const {return _responses;}
  unsigned response_errors()    const {return _response_errors;}
  unsigned instructions()       const {return _instructions;}
  unsigned instruction_errors() const {return _instruction_errors;}
public:
  void clear();
private:
  bool             _verbose;
  DAQ::LocationSet _sources;
  unsigned         _responses;
  unsigned         _response_errors;
  unsigned         _instructions;
  unsigned         _instruction_errors;
};
  
}

#endif
