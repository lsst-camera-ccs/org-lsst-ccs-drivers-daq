#include <stdio.h>
#include <stdlib.h>

#include "rms/Client.hh"
#include "SetLink.hh"

static const char USAGE[] = "usage: rms_set_link partition -d|-u|-r [duration] [DAQ Locations]\n";

#define PROGRAM   argv[0]
#define PARTITION argv[1]
#define TARGETS   argv[2]

int main(int argc, char** argv)
{
  if(argc < 2) {printf(USAGE); DAQ::LocationSet::usage(); return EXIT_SUCCESS;}

  RMS::Client client(PARTITION);
  RMS::Access::SetLink set_link(client);
  set_link.process(argc-2, (const char**)&TARGETS, 0);
  return EXIT_SUCCESS;
}
