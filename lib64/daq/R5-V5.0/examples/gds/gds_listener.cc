
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "dvi/TimeStamp.hh"

#include "gds/Subscriber.hh"
#include "gds/LocationSet.hh"

#include "Subscriber.hh"
#include "Decoder.hh"

const char USAGE[] = "usage: gds_listener <partition> [-d(ecode)] [-v(erbose)]\n";

int main(int argc, char** argv) 
{
  bool decode = false;
  bool verbose = false;

  int c;
  while(-1 != (c = getopt(argc, argv, "dv"))) {
    if('d' == c) decode = true;
    else if('v' == c) verbose = true;
  }

  if((argc - optind)<1) {printf(USAGE); return EXIT_SUCCESS;}

  char *partition = argv[optind++];

  GDS::LocationSet locs(GDS::Set::ANY);

  GDS::Subscriber* subscriber;
  if(decode)
    subscriber = new GDS::Example::Decoder(partition, locs);
  else
    subscriber = new GDS::Example::Subscriber(partition, locs, verbose);

  printf("Waiting on Guider Data Packets from %s\n", locs.encode());

  while(1) 
  {
    subscriber->wait();
  }

  delete subscriber;
}
