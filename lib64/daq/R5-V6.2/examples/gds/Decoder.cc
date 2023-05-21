#include "daq/Sensor.hh"

#include "Decoder.hh"

using namespace GDS;

Example::Decoder::Decoder(const char* partition, const GDS::LocationSet& locs) : 
  GDS::Decoder(partition, locs)
{

};

void Example::Decoder::start  (const GDS::StateMetadata& state, const GDS::SeriesMetadata& series)
{
  state.dump(); 
  series.dump();

  _stamp_size  = series.common().pixels()*sizeof(int32_t);
  _rstamp_size = RawStamp::calc_size(series);
}

void Example::Decoder::resume (const GDS::StateMetadata& state)
{
  state.dump();
  
  _begin [state.sensor().index()]        = state.timestamp();
  _stamps[state.sensor().index()]        = 0;
  _rstamps[state.sensor().index()]       = 0;
  _stamp_errors[state.sensor().index()]  = 0;
  _rstamp_errors[state.sensor().index()] = 0;
}

void Example::Decoder::pause  (const GDS::StateMetadata& state)
{
  state.dump();
  
  uint64_t begin = (uint64_t)_begin[state.sensor().index()];
  uint64_t   end = (uint64_t)state.timestamp();
  double    diff = (end-begin)/1E9;
  
  unsigned stamps        = _stamps[state.sensor().index()];
  unsigned rstamps       = _rstamps[state.sensor().index()];
  unsigned stamp_errors  = _stamp_errors[state.sensor().index()];
  unsigned rstamp_errors = _rstamp_errors[state.sensor().index()];
  double   freq          = rstamps/diff;
  printf("  %s: (%i) %i (raw) stamps in %f sec = %f Hz, Errors (raw, decoded) (%u, %u)\n", state.sensor().encode(), rstamps, stamps, diff, freq, rstamp_errors, stamp_errors);

}

void Example::Decoder::stop   (const GDS::StateMetadata& state)
{
  state.dump(); 

  unsigned stamps        = _stamps[state.sensor().index()];
  unsigned rstamps       = _rstamps[state.sensor().index()];
  unsigned stamp_errors  = _stamp_errors[state.sensor().index()];
  unsigned rstamp_errors = _rstamp_errors[state.sensor().index()];
  printf("  %s: (%i) %i (raw) stamps Errors (raw, decoded) (%u, %u)\n", state.sensor().encode(), rstamps, stamps, rstamp_errors, stamp_errors);
}

void Example::Decoder::raw_stamp(const GDS::StateMetadata& state, const GDS::RawStamp& stamp)
{
  ++_rstamps[state.sensor().index()];

  if(_rstamp_size != stamp.size()) {
    ++_rstamp_errors[state.sensor().index()];
    printf("ERROR - %s: Sequence %i, Stamp %i - Expected raw stamp size %u, recieved %u\n", state.sensor().encode(), state.sequence(), _rstamps[state.sensor().index()], _rstamp_size, stamp.size());
  }
}

void Example::Decoder::stamp(const GDS::StateMetadata& state, const GDS::Stamp& stamp)
{
  ++_stamps[state.sensor().index()];

  if(_stamp_size != stamp.size()) {
    ++_stamp_errors[state.sensor().index()];
    printf("ERROR - %s: Sequence %i, Stamp %i - Expected stamp size %u, recieved %u\n", state.sensor().encode(), state.sequence(), _stamps[state.sensor().index()], _stamp_size, stamp.size());
  }
}

uint8_t* Example::Decoder::allocate(unsigned size)
{
  return _stamp_buf;
}

