#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gds/Client.hh"

static const char USAGE[] = "usage:  gds_test <partition> <start|resume|pause|stop|config|series>\n";

#define PROGRAM   argv[0]
#define PARTITION argv[1]
#define COMMAND   argv[2]

#include "gds/RoiCommon.hh"
#include "gds/RoiLocation.hh"
#include "gds/Series.hh"

int main(int argc, char** argv)
{
  if(argc<3) {printf(USAGE); return EXIT_SUCCESS;}

  GDS::Client client(PARTITION);
  
  //GDS::RoiCommon common; // Default Parameters

  unsigned nrows = 50;
  unsigned ncols = 50;
  //unsigned integration = 5;
  unsigned integration = 50;
  //unsigned integration = 1000;
  unsigned binning = 1;
  GDS::RoiCommon common(nrows, ncols, integration, binning);

  GDS::RoiLocation locs[] = {
    //GDS::RoiLocation(GDS::Location(DAQ::Location("22/0"), 0),  2, 0, 0),
    //GDS::RoiLocation(GDS::Location(DAQ::Location("22/0"), 1), 10, 0, 0),
    GDS::RoiLocation(GDS::Location(DAQ::Location("22/1"), 0), 11, 0, 0),
    GDS::RoiLocation(GDS::Location(DAQ::Location("22/1"), 1), 13, 0, 0),
    //GDS::RoiLocation(GDS::Location(DAQ::Location("0/0"), 0),  2, 0, 0),
    //GDS::RoiLocation(GDS::Location(DAQ::Location("0/1"), 0), 10, 0, 0),
    //GDS::RoiLocation(GDS::Location(DAQ::Location("0/1"), 1), 11, 0, 0),
  };

  unsigned nlocs = sizeof(locs)/sizeof(GDS::RoiLocation);

  int error;
  GDS::Status response;
  GDS::Series series;
  GDS::RoiCommon combuf;
  GDS::RoiLocation locbuf[sizeof(GDS::RoiLocation)*GDS::LocationSet::SIZE];
  unsigned nlocsbuf;

  if(0==strcmp(COMMAND, "start")) {
    error = client.start(common, locs, nlocs, response);
    if(!response.status()) {
      common.dump(2);
      unsigned remaining = nlocs;
      const GDS::RoiLocation* loc = locs;
      printf("  Locations (%i):\n", nlocs);
      while(remaining--) loc++->dump(4);
    }
  }
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
    printf("command must be one of: wake, sleep, start, stop, pause, resume, series, config\n ");
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

