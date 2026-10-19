#include <stdio.h>
#include <stdlib.h>

#include "rms/Client.hh"

static const char USAGE[]      = "usage: rms_reset_reb partition [DAQ Locations]\n";
static const char NO_TARGETS[] = "No valid targets given\n";
static const char SUCCESS[]    = "%8s: Reset Succeeded\n";
static const char FAILURE[]    = "%8s: Reset FAILED\n";

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

  printf("Proceed with resetting %i REBs? [y/N]: ", targets.numof());
  fflush(stdin);
  char cont = getchar();
  if(('y' != cont) && ('Y' != cont)) return EXIT_SUCCESS;

  DAQ::LocationSet success = client.reset(targets);

  DAQ::Location loc;
  char buffer[16];
  while(targets.remove(loc))
  {
    const char* message = success.has(loc) ? SUCCESS : FAILURE;
    printf(message, loc.encodeReb(buffer));
  }

  return EXIT_SUCCESS;
}
