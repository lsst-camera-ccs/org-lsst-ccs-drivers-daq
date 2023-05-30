
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "dvi/TimeStamp.hh"

#include "gds/Subscriber.hh"
#include "gds/LocationSet.hh"

#include "Archiver.hh"

static const char DEFAULT_DIR[] = "./";

const char USAGE[] = "usage: gds_archiver <partition> [-d <output dir>] [-r(aw)]\n";

int main(int argc, char** argv)
{
  bool raw = false;
  const char* directory = DEFAULT_DIR;

  int c;
  while(-1 != (c = getopt(argc, argv, "rd:")))
  {
    if('r' == c) raw = true;
    if('d' == c) directory = optarg;
  }

  if(optind<1) {printf(USAGE); return 0;}

  const char* partition = argv[optind];

  printf("Archiving to %s\n", directory);

  //printf("sizeof(State) %i, sizeof(Series) %i\n", sizeof(GDS::StateMetadata), sizeof(GDS::SeriesMetadata));

  GDS::LocationSet locs(GDS::Set::ANY);

  //GDS::LocationSet locs;
  //locs.insert(GDS::Location(0, 1, 0));
  //locs.insert(GDS::Location(0, 1, 1));

  GDS::Example::Archiver subscriber(partition, locs, directory, raw);

  char buffer[4096];
  printf("Waiting on Guider Data Packets from %s\n", locs.encode(buffer));

  while(1) 
  {
    subscriber.wait();
  }

}
