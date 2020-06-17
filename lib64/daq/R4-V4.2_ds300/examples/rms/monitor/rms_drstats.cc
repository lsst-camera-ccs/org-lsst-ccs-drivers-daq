#include <stdio.h>
#include <stdlib.h>

#include "rms/Client.hh"
#include "Driver.hh"

static const char USAGE[] = "usage: rms_drstats partition [-h | -c] [DAQ Locations]\n";

#define PROGRAM   argv[0]
#define PARTITION argv[1]
#define TARGETS   argv[2]

int main(int argc, char** argv)
{
  if(argc < 2) {printf(USAGE); return EXIT_SUCCESS;}
  
  RMS::Client client(PARTITION);
  RMS::Monitor::Driver stats(client);
  stats.process(argc-2, (const char**)&TARGETS, 0);
  return EXIT_SUCCESS;
}

