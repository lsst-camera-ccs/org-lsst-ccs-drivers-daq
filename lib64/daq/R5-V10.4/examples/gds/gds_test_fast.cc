#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "gds/Client.hh"

static const char USAGE[] = "usage:  gds_test <partition> <start|resume|pause|stop|config|series>\n";

#define PROGRAM   argv[0]
#define PARTITION argv[1]
#define COMMAND   argv[2]

#include "gds/LocationSet.hh"
#include "gds/RoiCommon.hh"
#include "gds/RoiLocation.hh"
#include "gds/Series.hh"

int main(int argc, char** argv)
{
  if(argc<3) {printf(USAGE); return EXIT_SUCCESS;}

  GDS::Client client(PARTITION);

  unsigned nrows = 50;
  unsigned ncols = 50;
  //unsigned integration = 5;
  unsigned integration = 50;
  GDS::RoiCommon common(nrows, ncols, integration);

  GDS::RoiLocation locs[] = {
    //GDS::RoiLocation(GDS::Location(DAQ::Location("22/0"), 0), 15, 0, 0),
    //GDS::RoiLocation(GDS::Location(DAQ::Location("22/0"), 1), 9, 0, 0),
    //GDS::RoiLocation(GDS::Location(DAQ::Location("22/1"), 0),  7, 0, 0),
    //GDS::RoiLocation(GDS::Location(DAQ::Location("22/1"), 1),  11, 0, 0),
    //////GDS::RoiLocation(GDS::Location(DAQ::Location("22/1"), 1), 11, 1020, 320),
    //////GDS::RoiLocation(GDS::Location(DAQ::Location("22/1"), 1), 4, 1750, 300),
    GDS::RoiLocation(GDS::Location(DAQ::Location("0/0"), 0),  2, 100, 200),
    GDS::RoiLocation(GDS::Location(DAQ::Location("0/1"), 0), 10, 150, 250),
    GDS::RoiLocation(GDS::Location(DAQ::Location("0/1"), 1), 11, 200, 300),
    ///GDS::RoiLocation(GDS::Location(DAQ::Location("04/1"), 0), 10, 100, 200),
    ///GDS::RoiLocation(GDS::Location(DAQ::Location("04/1"), 1), 10, 200, 300),
    ///GDS::RoiLocation(GDS::Location(DAQ::Location("40/1"), 0),  7,  50, 150),
    ///GDS::RoiLocation(GDS::Location(DAQ::Location("40/1"), 1),  6,  50, 150),
    //
    /////////////
    // With Grid Projector at x=298 y=262
    // 50x50 ROIs get dots at:
    ////////////
    //GDS::RoiLocation(GDS::Location(DAQ::Location("22/1"), 1),  7,  25,  25),
    //GDS::RoiLocation(GDS::Location(DAQ::Location("22/1"), 0),  0, 495, 395),
    //GDS::RoiLocation(GDS::Location(DAQ::Location("22/0"), 1), 17,  50,  50),
    //GDS::RoiLocation(GDS::Location(DAQ::Location("22/0"), 0), 11,  25, 200),
    ////////////
    //
    //
    ////////////
    // 50x50 ROIs in ~center of Sensors
    //GDS::RoiLocation(GDS::Location(DAQ::Location("22/0"), 0),  0, 950, 205),
    //GDS::RoiLocation(GDS::Location(DAQ::Location("22/0"), 1),  0, 950, 205),
    //GDS::RoiLocation(GDS::Location(DAQ::Location("22/1"), 0),  0, 950, 205),
    //GDS::RoiLocation(GDS::Location(DAQ::Location("22/1"), 1),  0, 950, 205),
    //
    ////////////
    // x=299.5 y=342.2 puts a spot in a 50x50 ROI of 22/1/0 segment 0
    // x=257.0 y=341.8 puts a spot in a 50x50 ROI of 22/0/0 segment 0
    //
    // 50x50 ROIs no separation
    //GDS::RoiLocation(GDS::Location(DAQ::Location("22/1"), 0),  0, 0, 0),
    //GDS::RoiLocation(GDS::Location(DAQ::Location("22/1"), 1),  0, 0, 0),
    //
    // 50x50 ROIs maximum separation
    //GDS::RoiLocation(GDS::Location(DAQ::Location("22/1"), 0),  0, 0, 0),
    //GDS::RoiLocation(GDS::Location(DAQ::Location("22/1"), 1),  0, 1950, 459),
    //
    // 50x50 ROIs maximum separation, same rows
    //GDS::RoiLocation(GDS::Location(DAQ::Location("22/1"), 0),  0, 0,   0),
    //GDS::RoiLocation(GDS::Location(DAQ::Location("22/1"), 1),  0, 0, 459),
    //
    // 50x50 ROIs maximum separation, same cols
    //GDS::RoiLocation(GDS::Location(DAQ::Location("22/1"), 0),  0, 0,   0),
    //GDS::RoiLocation(GDS::Location(DAQ::Location("22/1"), 1),  0, 1950, 0),
    ////////////


  };

  unsigned nlocs = sizeof(locs)/sizeof(GDS::RoiLocation);

  int error;
  GDS::Status response;
  GDS::Series series;
  GDS::RoiCommon combuf;
  GDS::RoiLocation locbuf[sizeof(GDS::RoiLocation)*GDS::LocationSet::SIZE];
  unsigned nlocsbuf;

  uint32_t loops = 0;
  uint32_t clr_delay = 5000;
  GDS::LocationSet clr_locs;

  for(unsigned i=0; i<nlocs; ++i)
    clr_locs.insert(locs[i].location());

  while(true) {
    ++loops;

    client.wake(response);
    printf("%5i - ", loops); response.dump();
    if(response.status()) break;
    //usleep(500000);
    //sleep(1);

    client.start(common, locs, nlocs, "gregg_fast", 0, response);
    printf("%5i - ", loops); response.dump();
    if(response.status()) break;
    //usleep(500000);
    //sleep(1);

    client.resume(response);
    printf("%5i - ", loops); response.dump();
    if(response.status()) break;
    sleep(2);

    client.pause(response);
    printf("%5i - ", loops); response.dump();
    if(response.status()) break;
    //usleep(500000);
    //sleep(1);

    client.stop(response);
    printf("%5i - ", loops); response.dump();
    if(response.status()) break;
    //usleep(500000);
    //sleep(1);

    client.sleep(response);
    printf("%5i - ", loops); response.dump();
    if(response.status()) break;
    //usleep(500000);
    //sleep(1);
  }

  printf("Loops = %i\n", loops);
  
  return 0;

  if(0==strcmp(COMMAND, "start")) {
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
    error = client.clear(clr_locs, clr_delay, loops, response);
  else if(0==strcmp(COMMAND, "stop"))
    error = client.stop(response);
  else if(0==strcmp(COMMAND, "pause"))
    error = client.pause(response);
  else if(0==strcmp(COMMAND, "resume"))
    error = client.resume(response);
  else if(0==strcmp(COMMAND, "wake"))
    error = client.wake(response);
  else if(0==strcmp(COMMAND, "sleep"))
    error = client.sleep(response);
  else if(0==strcmp(COMMAND, "series"))
    error = client.series(response, series);
  else if(0==strcmp(COMMAND, "config"))
    error = client.config(response, series, combuf, locbuf, nlocsbuf);
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
    if(nlocsbuf)
    {
      combuf.dump(2);
      printf("Locations (%i):\n", nlocsbuf);
      const GDS::RoiLocation* loc = locbuf;
      unsigned remaining = nlocsbuf;
      while(remaining--) loc++->dump(2);
    }

  }

  return EXIT_SUCCESS;
}


