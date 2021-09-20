
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "osa/Timer.hh"
#include "osa/TimeStamp.hh"

#include "ims/Stream.hh"
#include "ims/Store.hh"
#include "ims/Image.hh"
#include "ims/Barrier.hh"

#include "editor/Inspector.hh"

#define PROGRAM   argv[0]
#define PARTITION argv[1]
#define SUMMARIZE argv[2]

#define IMAGE_TIMOUT  (15 * 1000 * 1000) // 15 seconds expressed in 1us tics... 
#define SOURCE_TIMOUT (10 * 1000 * 1000) // 10 seconds expressed in 1us tics... 

#define USECS (1000*1000) // usecs in 1 sec

using namespace IMS;

static const char BEGIN[]   = "Begin monitoring partition %s at %s\n";
static const char TIME[]    = "Image acquisition in %u.%-06u secs\n";
static const char HEADER[]  = "Timestamp, Folder/Name, Sources, Complete, Size, AcquisitionTime\n";
static const char SUMMARY[] = "%s, %s/%s, %i, %c, %lli, %u.%-06u\n";

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

  bool verbose = true;
  if(argc==3) verbose = strcmp(SUMMARIZE, "-s");

  Store store(PARTITION);

  Stream* stream = new Stream(store, SOURCE_TIMOUT);

  if(verbose) printf(BEGIN, PARTITION, OSA::TimeStamp().decode());
  else        printf(HEADER);

  while(1) 
  {
    Image image(store, *stream);
  
    if(!image) continue;
    
    OSA::Timer timer; timer.start();

    if(verbose) {Image::title(); image.synopsis();}
    
    Barrier barrier(image);
    
    barrier.block(*stream);

    long time = timer.stop();

    Editor::Inspector inspector(image, verbose);
    inspector.run();

    if(verbose) {
      inspector.summary();
      printf(TIME, time/USECS, time%USECS);
    }
    else {
      printf(SUMMARY,
	     image.metadata().timestamp().decode(),
	     image.metadata().folder(),
	     image.metadata().name(),
	     inspector.sources(),
	     inspector.complete() ? 'Y' : 'N',
	     inspector.size(),
	     time/USECS, time%USECS);
    }

    if(!inspector.complete())
    {
      delete stream;
      stream = new Stream(store, SOURCE_TIMOUT);
    }
    
  }

  return(EXIT_SUCCESS);
}
