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

static const DAQ::LocationSet _parse_targets(int argc, const char** argv);
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
  uint64_t vector = strtoull(VECTOR, &last, 0);
  if(last == VECTOR) {printf(USAGE); return EXIT_SUCCESS;}

  DAQ::LocationSet locs = _parse_targets(argc-3,(const char**)&TARGETS);

  unsigned tmo = 0; // in 10ms tics, 0 = wait forever

  if(RMS::LAM::Wait::TIMEOUT == wait.all(locs, vector, tmo))
    printf("Timed out waiting for all LAMs\n");
  
  return EXIT_SUCCESS;

}

const DAQ::LocationSet _parse_targets(int argc, const char** argv)
{
  
  if(0 == argc) { return DAQ::All();}

  bool exclude = false;
  
  DAQ::LocationSet targets;

  for(int i=0; i<argc; ++i)
  {
    DAQ::LocationSet set;
    if(0==strncmp(argv[i], "-a", 2))
    {
      set |= DAQ::All();
    }
    else if(0==strcmp(argv[i], "-x")) exclude = true;
    else if(0==strcmp(argv[i], "-s")) set  = DAQ::ScienceSet();
    else if(0==strcmp(argv[i], "-g")) set  = DAQ::GuidingSet();
    else if(0==strcmp(argv[i], "-w")) set  = DAQ::WavefrontSet();
    else                              set |= DAQ::LocationSet(1, &argv[i]);

    if(exclude) targets &= ~set;
    else        targets |=  set;
  }   

  return targets;
}

static const char HELP[] = \
  "  This utility is an example of using the RMS::LAM::Wait class' all method.\n"
  "  It will wait for, all bits of the provided LAM vector to be received from\n"
  "  the specified DAQ Locations."
  "\n"
  "  DAQ Locations can be specified in the forms: \n"
  "    <Raft[/REB]> (Examples: 10/0 10/1 22 0/1 00/1\n"
  "\n"
  "  The following predefined Sets are also supported:\n"
  "    -a All Sources\n"
  "    -s Science Sources\n"
  "    -w Wavefront Sources\n"
  "    -g Guiding Sources\n"
  "\n"
  "  Any of these forms can be combined, for example:\n"
  "    Select Raft22/REB0, Raft10, and Wavefonts: 22/0 10 -w\n"
  "\n"
  "  Sources can be excluded from previously defined sets by placing them after -x. \n"
  "  For Example:\n"
  "    Select all Sources except Guiding Sources:   -a -x -g\n"
  "    Select all Science Sources except Raft 10:   -s -x 10\n"
  "    Select all Corner Sources except Raft0/REB1: -w -g -x 0/1\n"
  "\n";

static void help() {printf(HELP);}


#include "dsid/Client.hh"

uint8_t _lookup(const char* partition)
{
  DSID::Client client;
  
  return client.lookup(partition);
}
