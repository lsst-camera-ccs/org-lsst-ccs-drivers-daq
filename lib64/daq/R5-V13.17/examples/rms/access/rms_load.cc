#include <stdio.h>
#include <stdlib.h>

#include "rms/Client.hh"
#include "rms/InstructionCache.hh"
#include "rms/HarvestStatus.hh"
#include "daq/LocationSet.hh"

static const char USAGE[] = "usage: rms_load <partition> <instruction cache file>  [DAQ Locations]\n";

#define PROGRAM   argv[0]
#define PARTITION argv[1]
#define FILENAME  argv[2]
#define LOCATIONS argv[3]

/*
** ++
**
**
** --
*/

int main(int argc, char* argv[])
{
  
  if(argc < 3) {printf(USAGE); DAQ::LocationSet::usage(); return EXIT_SUCCESS;}

  RMS::Client           client(PARTITION);
  RMS::InstructionCache cache(FILENAME);
  DAQ::LocationSet      locs(argc-3, (const char**)&LOCATIONS);
  locs &= client.sources();

  RMS::HarvestStatus status;
  client.load(locs, cache, status);

  status.summary();

  return EXIT_SUCCESS;
}
