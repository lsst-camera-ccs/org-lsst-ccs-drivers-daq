#ifndef RMS_RESULTS
#define RMS_RESULTS

#include "daq/LocationSet.hh"
#include "rms/InstructionList.hh"
#include "rms/Harvest.hh"

namespace RMS {

class Results : public Harvest
{
public:
  void process(const DAQ::Location&, const InstructionList&, int32_t error);
public:
  const DAQ::LocationSet& sources() const {return _sources;}
  const InstructionList&  result(const DAQ::Location& loc) const {return _results[loc.index()];}
  const int32_t           error (const DAQ::Location& loc) const {return _errors[loc.index()];}
private:
  DAQ::LocationSet _sources;
  InstructionList  _results[DAQ::LocationSet::SIZE];
  int32_t          _errors[DAQ::LocationSet::SIZE];
};
}
#endif
