#include <stdio.h>
#include <stdlib.h>

#include "rms/Client.hh"
#include "Write.hh"

static const char USAGE[] = "usage: rms_write partition address data [DAQ Locations]\n";

#define PROGRAM   argv[0]
#define PARTITION argv[1]
#define REGISTERS argv[2]
#define VALUE     argv[3]
#define TARGETS   argv[4]

/*
** ++
**
**
** --
*/

int main(int argc, char* argv[])
{
  
  if(argc < 4) {printf(USAGE); return EXIT_SUCCESS;}

  RMS::Client client(PARTITION);
  RMS::Access::Write write(client);
  write.process(argc-2, (const char**)&REGISTERS, 0);
  return EXIT_SUCCESS;
}
