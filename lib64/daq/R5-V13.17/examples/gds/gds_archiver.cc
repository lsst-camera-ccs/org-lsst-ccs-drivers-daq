
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "dvi/TimeStamp.hh"

#include "gds/Subscriber.hh"
#include "gds/LocationSet.hh"

#include "Archiver.hh"

static const char DEFAULT_DIR[] = "./";

const char USAGE[] = "usage: gds_archiver <partition> [-d <output dir>] [-r(aw)] [locations]\n";

int main(int argc, char** argv)
{
  bool raw = false;
  const char* directory = DEFAULT_DIR;

  int c;
  while(-1 != (c = getopt(argc, argv, "hrd:")))
  {
    if('r' == c) raw = true;
    if('d' == c) directory = optarg;
    if('h' == c) {printf(USAGE); return 0;}
  }

  if(optind<1 or argc<2) {printf(USAGE); return 0;}

  const char* partition = argv[optind++];

  setvbuf(stdout, NULL, _IOLBF, 0);

  printf("Archiving to %s\n", directory);

  int targets = argc-optind;
  GDS::LocationSet locs;
  if(0 == targets)
    locs = GDS::Set::ANY;
  else
    locs = GDS::LocationSet(targets, (const char**)&argv[optind]);

  GDS::Example::Archiver subscriber(partition, locs, directory, raw);

  char buffer[4096];
  printf("Waiting on Guider Data Packets from %s\n", locs.encode(buffer));

  while(1) 
  {
    subscriber.wait();
  }

}
