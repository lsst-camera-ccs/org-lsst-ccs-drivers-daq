#include <stdio.h>
#include <stdlib.h>

#include "rms/Client.hh"

static const char USAGE[]      = "usage: rms_reboot_rce partition [DAQ Locations]\n";
static const char NO_TARGETS[] = "No valid targets given\n";
static const char SUCCESS[]    = "Reboot RCE posted successfully\n";
static const char FAILURE[]    = "Reboot RCE post FAILED\n";

#define PROGRAM   argv[0]
#define PARTITION argv[1]
#define TARGETS   argv[2]

int main(int argc, char** argv)
{
  if(argc < 3) {printf(USAGE); DAQ::LocationSet::usage(); return EXIT_SUCCESS;}

  RMS::Client client(PARTITION);

  DAQ::LocationSet targets(argc-2, (const char**)&TARGETS);
  targets &= client.sources();

  if(!targets) {printf(NO_TARGETS); return EXIT_SUCCESS;}

  printf("Proceed with rebooting Readout RCEs for %i REBs? [y/N]: ", targets.numof());
  fflush(stdin);
  char cont = getchar();
  if(('y' != cont) && ('Y' != cont)) return EXIT_SUCCESS;

  bool success = client.reboot_rce(targets);

  printf(success ? SUCCESS : FAILURE);

  return EXIT_SUCCESS;
}
