#include <stdio.h>
#include <stdlib.h>

#include "rms/Client.hh"
#include "ReadTest.hh"

#define PROGRAM   argv[0]
#define PARTITION argv[1]
#define REGISTERS argv[2]
#define TARGETS   argv[3]

/*
** ++
**
**
** --
*/

int main(int argc, char* argv[])
{
  
  if(argc < 3) { RMS::Access::ReadTest::usage(); return EXIT_SUCCESS; }

  RMS::Client client(PARTITION);
  RMS::Access::ReadTest read(client);
  read.process(argc-2, (const char**)&REGISTERS, 0);
  return EXIT_SUCCESS;
}
