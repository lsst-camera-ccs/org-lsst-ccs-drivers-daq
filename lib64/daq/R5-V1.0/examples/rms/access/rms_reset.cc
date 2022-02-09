#include <stdio.h>
#include <stdlib.h>

#include "rms/Client.hh"
#include "Reset.hh"

static const char USAGE[]   = "usage: rms_reset partition [DAQ Locations]\n";

#define PROGRAM   argv[0]
#define PARTITION argv[1]
#define TARGETS   argv[2]

int main(int argc, char** argv)
{
  if(argc < 2) {printf(USAGE); return EXIT_SUCCESS;}
  
  RMS::Client client(PARTITION);
  RMS::Access::Reset reset(client);
  reset.process(argc-2, (const char**)&TARGETS, 0);
  return EXIT_SUCCESS;
}

