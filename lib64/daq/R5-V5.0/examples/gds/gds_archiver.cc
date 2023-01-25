
#include <stdio.h>
#include <string.h>

#include "dvi/TimeStamp.hh"

#include "gds/Subscriber.hh"
#include "gds/LocationSet.hh"

#include "Archiver.hh"

#define PROGRAM   argv[0]
#define PARTITION argv[1]
#define RAW       argv[2]

const char USAGE[] = "usage: gds_archiver <partition> [-r(aw)]\n";

int main(int argc, char** argv)
{
  bool raw = false;

  if(argc<2)
  {
    printf(USAGE);
    return 0;
  }
  if(argc>2)
  {
    if(0 == strncmp("-r", RAW, 2))
    {
      raw = true;
    }
  }

  //printf("sizeof(State) %i, sizeof(Series) %i\n", sizeof(GDS::StateMetadata), sizeof(GDS::SeriesMetadata));

  GDS::LocationSet locs(GDS::Set::ANY);

  //GDS::LocationSet locs;
  //locs.insert(GDS::Location(0, 1, 0));
  //locs.insert(GDS::Location(0, 1, 1));

  GDS::Example::Archiver subscriber(PARTITION, locs, raw);

  char buffer[4096];
  printf("Waiting on Guider Data Packets from %s\n", locs.encode(buffer));

  while(1) 
  {
    subscriber.wait();
  }

}
