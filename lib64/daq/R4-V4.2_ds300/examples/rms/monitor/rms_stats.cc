#include <stdio.h>
#include <stdlib.h>

#include "rms/Client.hh"
#include "Service.hh"

static const char USAGE[] = "usage: rms_stats partition [-h | -c] [DAQ Locations]\n";

#define PROGRAM   argv[0]
#define PARTITION argv[1]
#define TARGETS   argv[2]

int main(int argc, char** argv)
{
  if(argc < 2) {printf(USAGE); return EXIT_SUCCESS;}
  
  RMS::Client client(PARTITION);
  RMS::Monitor::Service service(client);
  service.process(argc-2, (const char**)&TARGETS, 0);
  return EXIT_SUCCESS;
}

