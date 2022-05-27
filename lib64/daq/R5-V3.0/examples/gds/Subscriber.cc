
#include <stdio.h>

#include "Subscriber.hh"

using namespace GDS;

Example::Subscriber::Subscriber(const char* partition, const GDS::LocationSet& locs) :
  GDS::Subscriber(partition, locs)
{

};

void Example::Subscriber::start  (const GDS::StateMetadata& state, const GDS::SeriesMetadata& series)
{
  state.dump(); 
  series.dump();
}

void Example::Subscriber::resume (const GDS::StateMetadata& state)
{
  state.dump();
  
  _begin [state.sensor().index()]     = state.timestamp();
  _stamps[state.sensor().index()]     = 0;
  _raw_stamps[state.sensor().index()] = 0;
}

void Example::Subscriber::pause  (const GDS::StateMetadata& state)
{
  state.dump();
  
  uint64_t begin = (uint64_t)_begin[state.sensor().index()];
  uint64_t   end = (uint64_t)state.timestamp();
  double    diff = (end-begin)/1E9;
  
  unsigned stamps     = _stamps[state.sensor().index()];
  unsigned raw_stamps = _raw_stamps[state.sensor().index()];
  double   freq       = raw_stamps/diff;
  printf("  %s: (%i) %i (raw) stamps in %f sec = %f Hz\n", state.sensor().encode(), raw_stamps, stamps, diff, freq);
}

void Example::Subscriber::stop   (const GDS::StateMetadata& state)
{
  state.dump(); 
}

void Example::Subscriber::raw_stamp(const GDS::StateMetadata& state, const GDS::RawStamp& stamp)
{
  ++_raw_stamps[state.sensor().index()];
}

