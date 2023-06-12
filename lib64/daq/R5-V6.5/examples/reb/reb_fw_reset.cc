#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "osa/Timer.hh"

#include "reb/EEprom.hh"

static const char USAGE[] = "usage: reb_fw_reset <partition> <location> [location ...] \n"
  " <location> [location ...] The series of DAQ Locations reset. (minimum of 1)\n\n";

int main(int argc, char** argv)
{
  int c;

  while(-1 != (c = getopt(argc, argv, ""))) {}

  if((argc - optind)<1) {printf(USAGE); return EXIT_SUCCESS;}

  char* partition = argv[optind++];

  REB::EEprom      client(partition);
  DAQ::LocationSet locs(argc-optind, (const char**)&argv[optind]);
  locs &= client.locations();

  OSA::Timer timer; timer.start();
  DAQ::LocationSet failed = client.reset(locs);
  DAQ::LocationSet passed(locs^failed);

  if(passed) printf("Successfully reset in %ld usecs: %s\n", timer.stop(), passed.encode());
  
  if(failed) printf("Failed to reset: %s\n", failed.encode());
  
  return EXIT_SUCCESS;
}

