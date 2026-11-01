
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "osa/Timer.hh"
#include "osa/TimeStamp.hh"

#include "ims/Stream.hh"
#include "ims/Store.hh"
#include "ims/Bucket.hh"
#include "xds/Event.hh"

#define PROGRAM   argv[0]
#define PARTITION argv[1]

#define USECS (1000*1000) // usecs in 1 sec

using namespace IMS;

static const char BEGIN[]   = "Begin monitoring partition %s at %s\n";
static const char TIME[]    = "Image acquisition in %u.%-06u secs\n";
static const char HEADER[]  = "Timestamp,ID,Folder/Name,Sources,Complete,Size,AcquisitionTime\n";
static const char SUMMARY[] = "%s, %016llX, %s/%s, %i, %c, %lli, %u.%-06u\n";

static const char USAGE[] = "usage: ims_stream_monitor <partition> [-s(ummary)]\n";

/*
** ++
**
**
** --
*/

int main(int argc, char* argv[])
{

  if(argc<2) {printf(USAGE); return EXIT_SUCCESS;}

  Store store(PARTITION);

  Stream* stream = new Stream(store);

  printf(HEADER);

  XDS::Event event;
  while(stream->wait(event))
  {
    if(event.bucket().type() == IMS::Bucket::IMAGE)
    {
      printf("Image %016llX\n", (long long unsigned)event.bucket().value());
      continue;
    }

    DAQ::Location loc(event.bucket().instance());
    char source[8];
    loc.encode(source);

    if(event.bucket().type() == IMS::Bucket::METADATA)
    {
      printf("Image %016llX Source %s\n", (long long unsigned)event.bucket().value(), source);
      continue;
    }

    if(event.bucket().type() == IMS::Bucket::DATA)
    {
      printf("Image %016llX Source %s last %i offset %016llX length %08X\n",
	     (long long unsigned)event.bucket().value(), source, event.marker(),
	     (long long unsigned)event.offset(), (unsigned)event.length());
    }
    
    //event.dump();
  }

  return(EXIT_SUCCESS);
}
