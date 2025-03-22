#include <stdio.h> 
#include <stdlib.h>
   
#include "rms/Client.hh"

#include "Shell.hh"

#define PROGRAM   argv[0]
#define PARTITION argv[1]
#define INTERFACE argv[2]

static const char DEFAULT[] = "";

static const char USAGE[] = "usage: rms_access partition\n";


int main(int argc, char** argv)
{
  
  RMS::Client client(PARTITION);

  RMS::Access::Shell shell(client);

  shell.start();
  
  return EXIT_SUCCESS;
}

