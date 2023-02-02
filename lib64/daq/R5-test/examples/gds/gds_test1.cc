#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "gds/Client.hh"
#include "gds/Series.hh"

static const char USAGE[] = "usage:  gds_test1 <partition>\n";

#define PROGRAM   argv[0]
#define PARTITION argv[1]

int main(int argc, char** argv)
{
  if(argc<2) {printf(USAGE); return EXIT_SUCCESS;}

  GDS::Client client(PARTITION);
  GDS::Status response;
  int error;

  GDS::Series series;

  printf("\n\n\n");
  sleep(5);

  error = client.resume(response);
  printf("first command returned %i\n", error);
  if(!error) response.dump();
  
  printf("\n\n\n");
  sleep(5);
  
  error = client.pause(response);
  printf("second command returned %i\n", error);
  if(!error) response.dump();

  return EXIT_SUCCESS;
}

