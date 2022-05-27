#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gds/Client.hh"

static const char USAGE[] = "usage:  gds_test <partition> <start|resume|pause|stop>\n";

#define PROGRAM   argv[0]
#define PARTITION argv[1]
#define COMMAND   argv[2]

#include "gds/RoiCommon.hh"
#include "gds/RoiLocation.hh"
#include "gds/Series.hh"

int main(int argc, char** argv)
{
  if(argc<2) {printf(USAGE); return EXIT_SUCCESS;}

  GDS::Client client(PARTITION);
  
  //GDS::RoiCommon common; // Default Parameters

  unsigned nrows = 50;
  unsigned ncols = 50;
  unsigned integration = 50;
  //unsigned integration = 2000;
  unsigned binning = 1;
  GDS::RoiCommon common(nrows, ncols, integration, binning);

  GDS::RoiLocation locs[] = {
    GDS::RoiLocation(GDS::Location(DAQ::Location("22/0"), 0),  2, 100, 200),
  //  GDS::RoiLocation(GDS::Location(DAQ::Location("22/0"), 1), 10, 125, 225),
  //  GDS::RoiLocation(GDS::Location(DAQ::Location("22/1"), 0), 11, 150, 250),
  //  GDS::RoiLocation(GDS::Location(DAQ::Location("22/1"), 1), 12, 175, 275),
  //  GDS::RoiLocation(GDS::Location(DAQ::Location("0/1"), 0),  2, 100, 200),
  //  GDS::RoiLocation(GDS::Location(DAQ::Location("0/1"), 1), 10, 300, 400),
  };

  unsigned nlocs = sizeof(locs)/sizeof(GDS::RoiLocation);

  int error;
  GDS::Status response;
  GDS::Series series;
  GDS::RoiCommon combuf;
  GDS::RoiLocation locbuf[sizeof(GDS::RoiLocation)*GDS::LocationSet::SIZE];
  unsigned nlocsbuf;

  if(0==strcmp(COMMAND, "start")) {
    common.dump(2);
    unsigned remaining = nlocs;
    const GDS::RoiLocation* loc = locs;
    printf("  Locations (%i):\n", nlocs);
    while(remaining--) loc++->dump(4);
    error = client.start(common, locs, nlocs, response);
  }
  else if(0==strcmp(COMMAND, "stop"))
    error = client.stop(response);
  else if(0==strcmp(COMMAND, "pause"))
    error = client.pause(response);
  else if(0==strcmp(COMMAND, "resume"))
    error = client.resume(response);
  else if(0==strcmp(COMMAND, "series"))
    error = client.series(response, series);
  else if(0==strcmp(COMMAND, "config"))
    error = client.config(response, series, combuf, locbuf, nlocsbuf);
  else
  {
    printf("command must be one of: start, stop, pause, resume, series\n ");
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

