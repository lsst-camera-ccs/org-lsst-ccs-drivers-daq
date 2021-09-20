#ifndef RMS_ACCESS_HARVEST
#define RMS_ACCESS_HARVEST

#include "rms/Harvest.hh"
#include "daq/LocationSet.hh"
#include "rms/InstructionList.hh"

namespace RMS { namespace Access {

class Harvest : public RMS::Harvest
{
public:
  Harvest() : RMS::Harvest(), _sources(), _response_errors(0), _instructions(0), _instruction_errors(0) {};
public:
  void process(const DAQ::Location&, const InstructionList&, int32_t error);
public:
  void summary();
public:
  void clear();
private:
  DAQ::LocationSet _sources;
  unsigned         _response_errors;
  unsigned         _instructions;
  unsigned         _instruction_errors;
};

}}

#endif
