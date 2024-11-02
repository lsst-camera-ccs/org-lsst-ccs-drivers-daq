#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gds/Client.hh"

static const char USAGE[] = "usage:  gds_test <partition> <start|resume|pause|stop|config|series>\n";

#define PROGRAM   argv[0]
#define PARTITION argv[1]
#define COMMAND   argv[2]
#define ROIFILE   argv[3]

#include "gds/LocationSet.hh"
#include "gds/RoiCommon.hh"
#include "gds/RoiLocation.hh"
#include "gds/RoiFile.hh"
#include "gds/Series.hh"

int main(int argc, char** argv)
{
  if(argc<3) {printf(USAGE); return EXIT_SUCCESS;}

  GDS::Client client(PARTITION);

  int error;
  GDS::Status response;
  GDS::Series series;
  GDS::RoiCommon common;
  char locbuf[sizeof(GDS::RoiLocation)*GDS::LocationSet::SIZE];
  GDS::RoiLocation* locs = (GDS::RoiLocation*)locbuf;
  unsigned nlocs;

  uint32_t loops = 0;
  GDS::LocationSet clr_locs;


  if(0==strcmp(COMMAND, "start")) {
    if(argc<4) {printf("start command requires ROI JSON file\n"); return EXIT_SUCCESS;}
    GDS::RoiFile rois(ROIFILE);
    if(!rois.next(common, locs, nlocs)) return EXIT_SUCCESS;
    error = client.start(common, locs, nlocs, "gregg_test", loops, response);
    if(!response.status()) {
      common.dump(2);
      unsigned remaining = nlocs;
      const GDS::RoiLocation* loc = locs;
      printf("  Locations (%i):\n", nlocs);
      while(remaining--) loc++->dump(4);
    }
  }
  else if(0==strcmp(COMMAND, "clear"))
  {
    if(argc<4) {printf("clear command requires ROI JSON file\n"); return EXIT_SUCCESS;}
    GDS::RoiFile rois(ROIFILE);
    if(!rois.next(common, locs, nlocs)) return EXIT_SUCCESS;

    for(unsigned i=0; i<nlocs; ++i)
    clr_locs.insert(locs[i].location());

    error = client.clear(clr_locs, common.integration(), loops, response);
  }
  else if(0==strcmp(COMMAND, "stop"))
    error = client.stop("stop comment", response);
  else if(0==strcmp(COMMAND, "pause"))
    error = client.pause("pause comment", response);
  else if(0==strcmp(COMMAND, "resume"))
    error = client.resume("resume comment", response);
  else if(0==strcmp(COMMAND, "wake"))
    error = client.wake(response);
  else if(0==strcmp(COMMAND, "sleep"))
    error = client.sleep(response);
  else if(0==strcmp(COMMAND, "series"))
    error = client.series(response, series);
  else if(0==strcmp(COMMAND, "config"))
    error = client.config(response, series, common, locs, nlocs);
  else
  {
    printf("command must be one of: wake, sleep, start, stop, pause, resume, series, config, clear\n ");
    return EXIT_SUCCESS;
  }
  
  if(error<0)
  {
    char buffer[256];
    printf("%s\n", GDS::Exception::decode(error, buffer));
  }
  else
  {
    response.dump();
    if(series)
    {
      printf("  Last transition: %s", response.state_out().dump());
      series.dump(2);
    }
  }

  return EXIT_SUCCESS;
}

