#ifndef CCS_HARVESTER
#define CCS_HARVESTER

#include "rms/Harvest.hh"
#include "rms/InstructionList.hh"
#include "daq/Location.hh"
#include "daq/LocationSet.hh"

class MyHarvester : public RMS::Harvest {
public:
    MyHarvester(int size);
public:
    ~MyHarvester() { delete _values; }
public:
    void process(const DAQ::Location& loc, const RMS::InstructionList& result, int32_t error);
    int errorCount() { return _errors.numof(); }
    int* values(int i) { return _values+i*_size; }
private:
    int _size;
    DAQ::LocationSet _all;
    DAQ::LocationSet _errors;
    int* _values;
};

#endif
