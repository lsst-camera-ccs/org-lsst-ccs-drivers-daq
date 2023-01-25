
#ifndef GDS_DECODER
#define GDS_DECODER

#include "gds/Subscriber.hh"
#include "gds/LocationSet.hh"
#include "gds/SeriesMetadata.hh"
#include "gds/StateMetadata.hh"
#include "gds/Stamp.hh"

#include <stdio.h>

namespace GDS {

class Decoder : public Subscriber
{
public:
  Decoder(const char* partition, const LocationSet&);
public:
  virtual ~Decoder();
public:
  void wait();
protected:
  virtual void stamp(const StateMetadata&, const Stamp&) {};
private:
  virtual uint8_t* allocate(unsigned size) = 0;
private:
  Stamp _decode(uint8_t* dest, const RawStamp&, const SeriesMetadata&) const;
private:
  SeriesMetadata _series[GDS::Set::SIZE];
};}

#endif
