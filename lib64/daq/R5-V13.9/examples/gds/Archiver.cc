
#include <string.h>

#include "osa/Timer.hh"

#include "Archiver.hh"

using namespace GDS;

Example::Archiver::Archiver(const char* partition, const GDS::LocationSet& locs, const char* output_dir, bool raw) :
  GDS::Decoder(partition, locs),
  _raw(raw)
{
  strncpy(_directory, output_dir, sizeof(_directory));

  int remaining = Set::SIZE;
  while(remaining--)
  {
    _dfile[remaining] = NULL;
    _rfile[remaining] = NULL;
  }
};

Example::Archiver::~Archiver()
{
  printf("here\n");
  int remaining = Set::SIZE;
  while(remaining--)
  {
    if(_dfile[remaining]) fclose(_dfile[remaining]);
    if(_rfile[remaining]) fclose(_rfile[remaining]);
  }
}

const char MODE[] = "wb";

void Example::Archiver::start  (const GDS::StateMetadata& state, const GDS::SeriesMetadata& series)
{
  OSA::Timer timer; timer.start();

  state.dump(); 

  int idx = state.sensor().index();

  _stamps[state.sensor().index()]        = 0;
  _rstamps[state.sensor().index()]       = 0;
  
  char dfile[512];
  sprintf(dfile, "%s/%05i_%02i_%1i_%1i.stamp",  _directory, state.sequence(), state.sensor().bay(), state.sensor().board(), state.sensor().sensor());
  _dfile[idx] = fopen(dfile, MODE); 

  fwrite(&state,  sizeof(StateMetadata),  1, _dfile[idx]);
  fwrite(&series, sizeof(SeriesMetadata), 1, _dfile[idx]);
  
  if(!_raw) return;

  char rfile[512];
  sprintf(rfile, "%s/%05i_%02i_%1i_%1i.rstamp", _directory, state.sequence(), state.sensor().bay(), state.sensor().board(), state.sensor().sensor());
  _rfile[idx] = fopen(rfile, MODE); 
  fwrite(&state,  sizeof(StateMetadata),  1, _rfile[idx]);
  fwrite(&series, sizeof(SeriesMetadata), 1, _rfile[idx]);

  printf("  - executed in %ld usecs\n", timer.stop());
}

void Example::Archiver::stop   (const GDS::StateMetadata& state)
{
  OSA::Timer timer; timer.start();

  int idx = state.sensor().index();
  if(_dfile[idx]) {fclose(_dfile[idx]); _dfile[idx]=NULL;}
  if(_rfile[idx]) {fclose(_rfile[idx]); _rfile[idx]=NULL;}

  state.dump(); 

  unsigned stamps        = _stamps[state.sensor().index()];
  unsigned rstamps       = _rstamps[state.sensor().index()];
  printf("  %s: (%i) %i (raw) stamps\n", state.sensor().encode(), rstamps, stamps);
  printf("  - executed in %ld usecs\n", timer.stop());
}

void Example::Archiver::raw_stamp(const GDS::StateMetadata& state, const GDS::RawStamp& stamp)
{
  if(!_raw) return;

  ++_rstamps[state.sensor().index()];

  FILE* file = _rfile[state.sensor().index()];
  fwrite(&state, sizeof(StateMetadata), 1, file);
  fwrite(stamp.content(), stamp.size(), 1, file);
}

void Example::Archiver::stamp(const GDS::StateMetadata& state, const GDS::Stamp& stamp)
{
  ++_stamps[state.sensor().index()];

  FILE* file = _dfile[state.sensor().index()];
  fwrite(&state, sizeof(StateMetadata), 1, file);
  fwrite(stamp.content(), stamp.size(), 1, file);
}

uint8_t* Example::Archiver::allocate(unsigned size)
{
  return _stamp_buf;
}

