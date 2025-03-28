#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "gds/Client.hh"

static const char USAGE[] = "usage:  gds_test_wakesleep <partition> <loops> <clearfile>\n";

#define PROGRAM   argv[0]
#define PARTITION argv[1]
#define LOOPS     argv[2]
#define CLEARFILE argv[3]

#include "gds/ClearParameters.hh"
#include "gds/ClearFile.hh"

int main(int argc, char** argv)
{
  if(argc<4) {printf(USAGE); return EXIT_SUCCESS;}

  GDS::Client client(PARTITION);

  uint32_t loops;
  if(1!=sscanf(LOOPS, "%d", &loops)) {printf(USAGE); return EXIT_SUCCESS;}

  GDS::Status response;

  GDS::ClearFile json(CLEARFILE);
  GDS::ClearParameters clear_params;
  json.fillParams(clear_params);

  int remaining = loops;
  int n=0;
  while(remaining--) {
    ++n;

    client.wake(clear_params, response);
    printf("%5i - ", n); response.dump();
    if(response.status()) break;
    //usleep(500000);
    sleep(1);

    client.sleep(response);
    printf("%5i - ", n); response.dump();
    if(response.status()) break;
    // usleep(500000);
    //sleep(1);
  }

  printf("Loops = %i\n", n);
  
  return EXIT_SUCCESS;
}


