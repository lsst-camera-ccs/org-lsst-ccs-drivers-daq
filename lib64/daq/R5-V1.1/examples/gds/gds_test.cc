#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gds/Client.hh"

static const char USAGE[] = "usage:  gds_test <start|resume|pause|stop>\n";

#define PROGRAM   argv[0]
#define COMMAND   argv[1]

#include "gds/RoiCommon.hh"
#include "gds/RoiLocation.hh"

int main(int argc, char** argv)
{

  if(argc<2) {printf(USAGE); return EXIT_SUCCESS;}

  GDS::Client client;
  
  //GDS::RoiCommon common; // Default Parameters

  unsigned nrows = 50;
  unsigned ncols = 50;
  unsigned integration = 50;
  unsigned binning = 1;
  GDS::RoiCommon common(nrows, ncols, integration, binning);

  //GDS::RoiLocation locs[1] = {
  //  GDS::RoiLocation(GDS::Location(DAQ::Location("0/1"), 0), 100, 200)
  //};

  GDS::RoiLocation locs[2] = {
    GDS::RoiLocation(GDS::Location(DAQ::Location("0/1"), 0),  2, 100, 200),
    GDS::RoiLocation(GDS::Location(DAQ::Location("0/1"), 1), 10, 300, 400)
  };

  unsigned nlocs = sizeof(locs)/sizeof(GDS::RoiLocation);

  int error;
  GDS::Status response;
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
  else
  {
    printf("command must be one of: start, stop, pause, resume\n ");
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
  }

  return EXIT_SUCCESS;
}

