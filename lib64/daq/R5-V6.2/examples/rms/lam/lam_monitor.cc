#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "daq/All.hh"
#include "daq/ScienceSet.hh"
#include "daq/GuidingSet.hh"
#include "daq/WavefrontSet.hh"

#include "rms/lam/Wait.hh"
#include "rms/lam/Vector.hh"
#include "rms/lam/Address.hh"

static const char USAGE[] = "usage: rms_lam_monitor partition [vector] [DAQ Locations...]\n";

static uint8_t _lookup(const char* partition);
static void help();

#define PROGRAM   argv[0]
#define PARTITION argv[1]
#define VECTOR    argv[2]
#define TARGETS   argv[3]

#define INTERFACE "lsst-daq"

int main(int argc, char** argv)
{
  if(argc < 2) {printf(USAGE); help(); return EXIT_SUCCESS;}

  RMS::LAM::Address service(_lookup(PARTITION));
  
  RMS::LAM::Wait wait(service, INTERFACE);
  RMS::LAM::Vector lam;
  DAQ::Location loc;

  unsigned tmo = 0; // in 10ms tics, 0 = wait forever

  if(argc == 2)
  {
    while(true) 
    {
      if(RMS::LAM::Wait::TIMEOUT == wait.next(loc, lam, tmo)) break;
      printf("%s - %s\n", loc.encode(), lam.encode(true));
    }
    return EXIT_SUCCESS;
  }

  char* last;
  uint64_t vector = strtoull(VECTOR, &last, 0);
  if(last == VECTOR) {printf(USAGE); return EXIT_SUCCESS;}

  DAQ::LocationSet locs(argc-3,(const char**)&TARGETS);

  while(true)
  {
    if(RMS::LAM::Wait::TIMEOUT == wait.next(loc, lam, locs, vector, tmo)) break;
    printf("%s - %s\n", loc.encode(), lam.encode(true));
  }
  
  return EXIT_SUCCESS;

}

static const char HELP[] = \
  "  This utility is an example of using the RMS::LAM::Wait class' next methods.\n"
  "  It will wait for, and dump, LAMs as they arrive from the REBs.\n"
  "  Filters can be applied to both the LAM Vector and the DAQ Locations that can be printed.\n"
  "\n";

static void help()
{
  printf(HELP);
  DAQ::LocationSet::usage();
}


#include "dsid/Client.hh"

uint8_t _lookup(const char* partition)
{
  DSID::Client client;
  
  return client.lookup(partition);
}
