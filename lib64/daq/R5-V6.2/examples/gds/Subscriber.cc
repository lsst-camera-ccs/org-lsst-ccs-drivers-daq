
#include <stdio.h>

#include "Subscriber.hh"
#include "daq/Sensor.hh"

using namespace GDS;

Example::Subscriber::Subscriber(const char* partition, const GDS::LocationSet& locs, bool verbose) :
  GDS::Subscriber(partition, locs),
  _verbose(verbose)
{

};

void Example::Subscriber::start  (const GDS::StateMetadata& state, const GDS::SeriesMetadata& series)
{
  state.dump(); 
  series.dump();

  _rstamp_size = RawStamp::calc_size(series);
}

void Example::Subscriber::resume (const GDS::StateMetadata& state)
{
  state.dump();
  
  _begin [state.sensor().index()]     = state.timestamp();
  _raw_stamps[state.sensor().index()] = 0;
}

void Example::Subscriber::pause  (const GDS::StateMetadata& state)
{
  state.dump();
  
  uint64_t begin = (uint64_t)_begin[state.sensor().index()];
  uint64_t   end = (uint64_t)state.timestamp();
  double    diff = (end-begin)/1E9;
  
  unsigned raw_stamps = _raw_stamps[state.sensor().index()];
  double   freq       = raw_stamps/diff;
  printf("  %s:  %i raw stamps in %f sec = %f Hz\n", state.sensor().encode(), raw_stamps, diff, freq);
}

void Example::Subscriber::stop   (const GDS::StateMetadata& state)
{
  state.dump(); 

  unsigned raw_stamps = _raw_stamps[state.sensor().index()];
  printf("  %s: %i raw stamps\n", state.sensor().encode(), raw_stamps);
}

void Example::Subscriber::raw_stamp(const GDS::StateMetadata& state, const GDS::RawStamp& stamp)
{
  ++_raw_stamps[state.sensor().index()];

  if(_rstamp_size != stamp.size()) {
    printf("ERROR - %s: Sequence %i, Stamp %i - Expected raw stamp size %u, recieved %u\n", state.sensor().encode(), state.sequence(), _raw_stamps[state.sensor().index()], _rstamp_size, stamp.size());
  }
  if(!_verbose) return;

  char buf[16];
  printf("%016llx - %s Sensor %s: Sequence %i Stamp %i %s %s %i bytes\n", (unsigned long long)(uint64_t)state.timestamp(), state.timestamp().decode(), state.sensor().encode(buf), state.sequence(), _raw_stamps[state.sensor().index()], GDS::StateMetadata::type(state.type()), GDS::StateMetadata::status(state.status()), stamp.size());
}

