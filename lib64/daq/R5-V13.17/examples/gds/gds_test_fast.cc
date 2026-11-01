#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "gds/Client.hh"

static const char USAGE[] = "usage:  gds_test_fast <partition> <loops> <roifile>\n";

#define PROGRAM   argv[0]
#define PARTITION argv[1]
#define LOOPS     argv[2]
#define ROIFILE   argv[3]

#include "gds/LocationSet.hh"
#include "gds/RoiCommon.hh"
#include "gds/RoiLocation.hh"
#include "gds/RoiFile.hh"

#include "osa/TimeStamp.hh"

int main(int argc, char** argv)
{
  if(argc<4) {printf(USAGE); return EXIT_SUCCESS;}

  GDS::Client client(PARTITION);

  uint32_t loops;
  if(1!=sscanf(LOOPS, "%d", &loops)) {printf(USAGE); return EXIT_SUCCESS;}

  GDS::Status response;
  GDS::RoiCommon common;
  char locbuf[sizeof(GDS::RoiLocation)*GDS::LocationSet::SIZE];
  GDS::RoiLocation* locs = (GDS::RoiLocation*)locbuf;
  unsigned nlocs;

  GDS::RoiFile rois(ROIFILE);

  while(rois.next(common, locs, nlocs))
  {
    int remaining = loops;
    int n=0;
    while(remaining--) {
      ++n;

      OSA::TimeStamp before;
      client.start(common, locs, nlocs, "gregg_fast", GDS::Client::INFINITE_LOOP, response);
      OSA::TimeStamp after;
      printf("start time - %lld ms\n", (unsigned long long)((uint64_t)after-(uint64_t)before)/1000000);
      printf("%5i - ", n); response.dump();
      if(response.status()) break;
      // usleep(200000);
      // sleep(2);

      client.resume(response);
      printf("%5i - ", n); response.dump();
      if(response.status()) break;
      sleep(15);

      client.pause(response);
      printf("%5i - ", n); response.dump();
      if(response.status()) break;
      //usleep(500000);
      //sleep(1);

      client.stop(response);
      printf("%5i - ", n); response.dump();
      if(response.status()) break;
      //usleep(500000);
      // sleep(2);
    }

    printf("Loops = %i\n", n);
  }
  
  return EXIT_SUCCESS;
}


