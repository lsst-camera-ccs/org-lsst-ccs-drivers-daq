
#ifndef GDS_DECODER
#define GDS_DECODER

#include "gds/Subscriber.hh"
#include "gds/LocationSet.hh"
#include "gds/SeriesMetadata.hh"
#include "gds/StateMetadata.hh"
#include "gds/Stamp.hh"

#include <stdio.h>

namespace GDS {

namespace Stream {class RawBuffer;}

class Decoder : public Subscriber
{
public:
  Decoder(const char* partition, const LocationSet&, bool forceBE=false);
public:
  virtual ~Decoder();
private:
  virtual void     stamp(const StateMetadata&, const Stamp&) {};
  virtual uint8_t* allocate(unsigned size) = 0;
private:
  void  _stamp(const StateMetadata&, const Stream::RawBuffer&);
  Stamp _decode(uint8_t* dest, const RawStamp&, const SeriesMetadata&) const;
private:
  bool _forceBE;
};}

#endif
