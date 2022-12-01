#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "osa/Timer.hh"

#include "reb/EEprom.hh"

//static const char USAGE[] = "usage: reb_fw_load partition <slot> <bitfile> <location> [location ...] \n";
static const char USAGE[] = "usage: reb_fw_load <partition> -s <slot> -f file <location> [location ...] \n"
  " -s <slot>   The EEPROM slot to load\n"
  " -f <file>   The bitfile to load\n"
  " <location> [location ...] The series of DAQ Locations to boot. (minimum of 1)\n";

int main(int argc, char** argv)
{
  unsigned slot = 4;
  char *filename = NULL;

  int c;
  while(-1 != (c = getopt(argc, argv, "s:f:"))) {
    if     ('s' == c) slot     = atoi(optarg);
    else if('f' == c) filename = optarg;
  }

  if(NULL == filename || 4 == slot || (argc - optind)<2) {printf(USAGE); return EXIT_SUCCESS;}

  char* partition = argv[optind++];

  REB::EEprom      client(partition);
  DAQ::LocationSet locs(argc-optind, (const char**)&argv[optind]);

  OSA::Timer timer; timer.start();
  DAQ::LocationSet failed = client.load(locs, slot, filename);
  DAQ::LocationSet passed(locs^failed);

  if(passed) printf("Successfully loaded slot %i in %ld usecs: %s\n", slot, timer.stop(), passed.encode());
  
  if(failed) printf("Failed to load to slot %i: %s\n", slot, failed.encode());
  
  return EXIT_SUCCESS;
}

