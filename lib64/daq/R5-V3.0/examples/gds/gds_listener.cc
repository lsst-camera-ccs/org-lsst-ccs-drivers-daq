
#include <stdio.h>
#include <string.h>

#include "dvi/TimeStamp.hh"

#include "gds/Subscriber.hh"
#include "gds/LocationSet.hh"

#include "Subscriber.hh"
#include "Decoder.hh"

#define PROGRAM   argv[0]
#define PARTITION argv[1]
#define DECODE    argv[2]

const char USAGE[] = "usage: gds_listener <partition> [-d(ecode)]\n";

int main(int argc, char** argv)
{
  bool decode = false;

  if(argc<2)
  {
    printf(USAGE);
    return 0;
  }
  else if(argc>2)
  {
    if(0 == strncmp("-d", DECODE, 2)) decode = true;
  }

  GDS::LocationSet locs(GDS::Set::ANY);

  //GDS::LocationSet locs;
  //locs.insert(GDS::Location(0, 1, 0));
  //locs.insert(GDS::Location(0, 1, 1));

  GDS::Subscriber* subscriber;
  if(decode)
    subscriber = new GDS::Example::Decoder(PARTITION, locs);
  else
    subscriber = new GDS::Example::Subscriber(PARTITION, locs);

  //GDS::Example::Decoder subscriber(PARTITION, locs);

  printf("Waiting on Guider Data Packets from %s\n", locs.encode());

  while(1) 
  {
    subscriber->wait();
  }

  delete subscriber;
}
