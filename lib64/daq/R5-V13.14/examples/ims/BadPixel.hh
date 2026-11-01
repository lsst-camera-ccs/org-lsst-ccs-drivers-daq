#ifndef IMS_EDITOR_BADPIXEL
#define IMS_EDITOR_BADPIXEL

#include <stdint.h>

#include "ims/Decoder.hh"
#include "ims/Image.hh"

#include "ScienceData.hh"
#include "GuidingData.hh"
#include "WavefrontData.hh"

namespace IMS {namespace Editor {

class BadPixel : public IMS::Decoder {
public:
  enum : int32_t  {DEFAULT_LOW = 10000, DEFAULT_HIGH = 240000};
  enum : unsigned {DEFAULT_MAX_DUMP = 100};
public:
  BadPixel()              = delete; 
  BadPixel(const BadPixel&) = delete;  
public:
  BadPixel(Image&, int32_t low=DEFAULT_LOW, int32_t high=DEFAULT_HIGH);
  BadPixel(Image&, const DAQ::LocationSet& filter, int32_t low=DEFAULT_LOW, int32_t high=DEFAULT_HIGH);
public:
 ~BadPixel() {} 
public:
  void enable_dump(unsigned max_dump=DEFAULT_MAX_DUMP);
public:
  void process(Science::Source&,   uint64_t length, uint64_t base);
  void process(Guiding::Source&,   uint64_t length, uint64_t base);
  void process(Wavefront::Source&, uint64_t length, uint64_t base);
public:
  uint64_t  bad(const DAQ::Location& loc) const {return  _bad[loc.index()];}
  uint64_t total(const DAQ::Location& loc) const {return _total[loc.index()];}
public:
  uint64_t  bad() const;
  uint64_t total() const;
public:
  unsigned    dumped()    const {return _dumped;}
  unsigned bad_stripes()  const {return _bad_stripes;}
public:
  void dump(int indent=0) const;
  void dump(const DAQ::Location&, int indent=0) const;
private:
  void _count(const Stripe* stripes, uint64_t samples, const DAQ::Location& loc, unsigned xor_mask, unsigned ccd, uint64_t sample_base);
  void _dump_stripe(const Stripe&, unsigned xor_mask, bool flagged) const;
private:
  int32_t          _low;
  int32_t          _high;
  bool             _dump;
  unsigned         _max_dump;
  unsigned         _dumped;
  unsigned         _bad_stripes;
  DAQ::LocationSet _locs;
  ScienceData      _science;
  GuidingData      _guiding;
  WavefrontData    _wavefront;
  uint64_t         _bad  [DAQ::LocationSet::SIZE];
  uint64_t         _total[DAQ::LocationSet::SIZE];
};
 
}}

#endif
