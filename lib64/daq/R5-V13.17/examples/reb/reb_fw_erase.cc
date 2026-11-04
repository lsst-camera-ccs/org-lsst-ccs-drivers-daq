#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "osa/Timer.hh"

#include "daq/LocationSet.hh"
#include "reb/EEprom.hh"

static const char USAGE[] = "usage: reb_fw_erase <partition> -s <slot> -- <location> [location ...] \n"
  " -s <slot>   The EEPROM slot to erase\n"
  " <location> [location ...] The series of DAQ Locations to erase. (minimum of 1)\n\n";

int main(int argc, char** argv)
{
  unsigned slot = 4;

  int c;
  while(-1 != (c = getopt(argc, argv, "s:")))
    if('s' == c) slot = atoi(optarg);

  if(4 == slot || (argc - optind)<2) {printf(USAGE); DAQ::LocationSet::usage(); return EXIT_SUCCESS;}

  char* partition = argv[optind++];

  REB::EEprom      client(partition);
  DAQ::LocationSet locs(argc-optind, (const char**)&argv[optind]);
  locs &= client.locations();

  bool final = true;

  printf("Proceed with erasing slot %i on %i REBs? [y/N]: ", slot, locs.numof()); fflush(stdin);
  char cont = getchar();
  if(('y'!=cont) && ('Y'!=cont)) return EXIT_SUCCESS;

  OSA::Timer timer; timer.start();
  DAQ::LocationSet failed = client.erase(locs, slot, final);
  DAQ::LocationSet passed(locs^failed);

  if(passed) printf("Successfully erased %i REBs slot %i in %ld usecs: %s\n", passed.numof(), slot, timer.stop(), passed.encode());
  
  if(failed) printf("Failed to erase %i REBs slot %i: %s\n", failed.numof(), slot, failed.encode());
  
  return EXIT_SUCCESS;
}

