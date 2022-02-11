
#include <stdio.h>

#include "dvi/TimeStamp.hh"

#include "gds/Subscriber.hh"
#include "gds/LocationSet.hh"
#include "gds/StateMetadata.hh"
#include "gds/SeriesMetadata.hh"
#include "gds/RawStamp.hh"

/*
** ++
** 
** This is a trivial example of a Subsciber to the Guider Data Stream
** I suspect in actual use, a Subscriber (thread) will only listen on one stream
** but this example shows listening to an arbitrary number.
**
** --
*/

class Subscriber : public GDS::Subscriber 
{
public:
  Subscriber(const char* partition, const GDS::LocationSet& locs) : 
    GDS::Subscriber(partition, locs) {};
public:
  void start  (const GDS::StateMetadata& state, const GDS::SeriesMetadata& series) {
    state.dump(); 
    series.dump();
  }

  void resume (const GDS::StateMetadata& state) {
    state.dump();

    _begin [state.sensor().index()] = state.timestamp(); 
    _stamps[state.sensor().index()] = 0; 
  }

  void pause  (const GDS::StateMetadata& state) {
    state.dump();
    
    uint64_t begin = (uint64_t)_begin[state.sensor().index()];
    uint64_t   end = (uint64_t)state.timestamp();
    double    diff = (end-begin)/1E9;

    unsigned stamps = _stamps[state.sensor().index()];
    double    freq  = stamps/diff;
    printf("  %s: %i stamps in %f sec = %f Hz\n", state.sensor().encode(), stamps, diff, freq);
  }

  void stop   (const GDS::StateMetadata& state) {
    state.dump(); 
  }

  void stamp  (const GDS::StateMetadata& state, const GDS::RawStamp& stamp) {
    ++_stamps[state.sensor().index()];
  }

private:
  DVI::TimeStamp _begin[GDS::Set::SIZE];
  unsigned       _stamps[GDS::Set::SIZE];
};

/*
** ++
**
** Here is some test code to use the above.
**
** --
*/

#define PROGRAM   argv[0]
#define COMMAND   argv[1]

#include "gds/Partition.hh"
#include "stream/Header.hh"

int main(int argc, char** argv)
{

  GDS::LocationSet locs(GDS::Set::ANY);

  //GDS::LocationSet locs();
  //locs.insert(GDS::Location(0, 1, 0));
  //locs.insert(GDS::Location(0, 1, 1));

  Subscriber subscriber(GDS::PARTITION, locs);
  
  char buffer[4096];
  printf("Waiting on Guider Data Packets from %s\n", locs.encode(buffer));

  while(1) 
  {
    subscriber.wait();
  }

}
