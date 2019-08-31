#include <stdio.h> 
#include <stdlib.h>
   
#include "rms/Client.hh"

#include "Shell.hh"

#define PROGRAM   argv[0]
#define PARTITION argv[1]
#define INTERFACE argv[2]

static const char DEFAULT[] = "";

static const char USAGE[] = "usage: rms_monitor partition [network]\n";


int main(int argc, char** argv)
{
  
  const char* network;
  const char* partition;
  
  switch(--argc) {
  case 0: printf(USAGE);                              return EXIT_SUCCESS;
  case 1: network = DEFAULT;   partition = PARTITION; break;
  case 2: network = INTERFACE; partition = PARTITION; break;
  }
  
  RMS::Client client(partition, network);

  RMS::Monitor::Shell shell(client);
  
  shell.start();
  
  return EXIT_SUCCESS;
}

