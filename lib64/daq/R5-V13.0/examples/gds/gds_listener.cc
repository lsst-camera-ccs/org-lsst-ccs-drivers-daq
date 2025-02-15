
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <csignal>

#include "dvi/TimeStamp.hh"

#include "gds/Subscriber.hh"
#include "gds/LocationSet.hh"

#include "Subscriber.hh"
#include "Decoder.hh"

const char USAGE[] = "usage: gds_listener <partition> [-d(ecode)] [-v(erbose)] [locations]\n";

static GDS::Subscriber* subscriber;

void quitIt(int s)
{
  printf("received signal %i\n", s);
  if(s == 6 && subscriber)
  {
    printf("Aborting subscriber\n");
    subscriber->abort();
  }
}


int main(int argc, char** argv)
{
  struct sigaction sigIntHandler;
  sigIntHandler.sa_handler = quitIt;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;
  sigaction(SIGABRT, &sigIntHandler, NULL);

  bool decode = false;
  bool verbose = false;

  int c;
  while(-1 != (c = getopt(argc, argv, "dv"))) {
    if('d' == c) decode = true;
    else if('v' == c) verbose = true;
  }

  if((argc - optind)<1) {printf(USAGE); return EXIT_SUCCESS;}

  char *partition = argv[optind++];

  int targets = argc-optind;
  GDS::LocationSet locs;
  if(0 == targets)
    locs = GDS::Set::ANY;
  else
    locs = GDS::LocationSet(targets, (const char**)&argv[optind]);

  if(decode)
    subscriber = new GDS::Example::Decoder(partition, locs);
  else
    subscriber = new GDS::Example::Subscriber(partition, locs, verbose);

  printf("Waiting on Guider Data Packets from %s\n", locs.encode());

  while(*subscriber) subscriber->wait();

  delete subscriber;

}
