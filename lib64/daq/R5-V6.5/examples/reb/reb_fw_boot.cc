#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "osa/Timer.hh"

#include "reb/EEprom.hh"

static const char USAGE[] = "usage: reb_fw_boot <partition> -s <slot> <location> -- [location ...] \n"
  " -s <slot>   The EEPROM slot to boot from\n"
  " <location> [location ...] The series of DAQ Locations to boot. (minimum of 1)\n\n";

int main(int argc, char** argv)
{
  unsigned slot = 4;

  int c;
  while(-1 != (c = getopt(argc, argv, "s:")))
    if('s' == c) slot = atoi(optarg);

  if(4 == slot || (argc - optind)<2) {printf(USAGE); return EXIT_SUCCESS;}

  char* partition = argv[optind++];

  REB::EEprom      client(partition);
  DAQ::LocationSet locs(argc-optind, (const char**)&argv[optind]);
  locs &= client.locations();

  OSA::Timer timer; timer.start();
  DAQ::LocationSet failed = client.boot(locs, slot);
  DAQ::LocationSet passed(locs^failed);

  if(passed) printf("Successfully booted to slot %i in %ld usecs: %s\n", slot, timer.stop(), passed.encode());
  
  if(failed) printf("Failed to boot to slot %i: %s\n", slot, failed.encode());
  
  return EXIT_SUCCESS;
}

