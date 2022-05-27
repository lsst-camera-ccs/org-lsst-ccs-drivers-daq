#ifndef GDS_EXAMPLE_SUBSCRIBER
#define GDS_EXAMPLE_SUBSCRIBER

#include "dvi/TimeStamp.hh"
#include "gds/Subscriber.hh"

namespace GDS { namespace Example {

class Subscriber : public GDS::Subscriber
{
public:
  Subscriber(const char* partition, const GDS::LocationSet& locs);
private:
  void start    (const GDS::StateMetadata& state, const GDS::SeriesMetadata& series);
  void resume   (const GDS::StateMetadata& state);
  void pause    (const GDS::StateMetadata& state);
  void stop     (const GDS::StateMetadata& state);
  void raw_stamp(const GDS::StateMetadata& state, const GDS::RawStamp& stamp);
private:
  DVI::TimeStamp _begin[GDS::Set::SIZE];
  unsigned       _stamps[GDS::Set::SIZE];
  unsigned       _raw_stamps[GDS::Set::SIZE];
};

}}

#endif
