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

static const char USAGE[] = "usage: rms_lam_wait <partition> <vector> <DAQ Location> [DAQ Locations...]\n";

static uint8_t _lookup(const char* partition);
static void help();

#define PROGRAM   argv[0]
#define PARTITION argv[1]
#define VECTOR    argv[2]
#define TARGETS   argv[3]

#define INTERFACE "lsst-daq"

int main(int argc, char** argv)
{
  if(argc < 4) {printf(USAGE); help(); return EXIT_SUCCESS;}

  RMS::LAM::Address service(_lookup(PARTITION));
  
  RMS::LAM::Wait wait(service, INTERFACE);
  RMS::LAM::Vector lam;

  char* last;
  RMS::LAM::Vector vector(strtoull(VECTOR, &last, 0));
  if(last == VECTOR) {printf(USAGE); return EXIT_SUCCESS;}

  DAQ::LocationSet locs(argc-3,(const char**)&TARGETS);

  unsigned tmo = 0; // in 10ms tics, 0 = wait forever

  if(RMS::LAM::Wait::TIMEOUT == wait.all(locs, vector, tmo))
    printf("Timed out waiting for all LAMs\n");
  
  return EXIT_SUCCESS;

}

static const char HELP[] = \
  "  This utility is an example of using the RMS::LAM::Wait class' all method.\n"
  "  It will wait for, all bits of the provided LAM vector to be received from\n"
  "  the specified DAQ Locations.\n"
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
