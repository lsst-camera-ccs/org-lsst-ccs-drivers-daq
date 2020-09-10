#ifndef CCS_HARVESTER
#define CCS_HARVESTER

#include "rms/Harvest.hh"
#include "rms/InstructionList.hh"
#include "daq/Location.hh"
#include "daq/LocationSet.hh"

class MyHarvester : public RMS::Harvest {
public:
    MyHarvester();
public:
    ~MyHarvester() {}
public:
    void process(const DAQ::Location& loc, const RMS::InstructionList& result, int32_t error);
    DAQ::LocationSet errors() { return _errors; }
    int* values() { return _values; }
private:
    DAQ::LocationSet _all;
    DAQ::LocationSet _errors;
    int _values[4*25];
};

#endif
