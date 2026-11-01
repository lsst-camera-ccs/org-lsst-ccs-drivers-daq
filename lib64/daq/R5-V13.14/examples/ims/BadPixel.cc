#include <stdio.h>
#include <string.h>

#include "ims/Exception.hh"

#include "BadPixel.hh"

using namespace IMS;

static const char ERROR[] = "Error decoding element: %s (%s)\n";

/*
** ++
**
**
** --
*/

#define SAMPLES 195072

Editor::BadPixel::BadPixel(Image& image, int32_t low, int32_t high) :
 IMS::Decoder(image),
 _low(low),
 _high(high),
 _dump(false),
 _max_dump(DEFAULT_MAX_DUMP),
 _dumped(0),
 _bad_stripes(0),
 _locs(),
 _science(SAMPLES),
 _guiding(SAMPLES),
 _wavefront(SAMPLES),
 _bad{},
 _total{}
 {
 }

/*
** ++
**
**
** --
*/

Editor::BadPixel::BadPixel(Image& image, const DAQ::LocationSet& filter, int32_t low, int32_t high) :
 IMS::Decoder(image, filter),
 _low(low),
 _high(high),
 _dump(false),
 _max_dump(DEFAULT_MAX_DUMP),
 _dumped(0),
 _bad_stripes(0),
 _locs(),
 _science(SAMPLES),
 _guiding(SAMPLES),
 _wavefront(SAMPLES),
 _bad{},
 _total{}
 {
 }

/*
** ++
**
**
** --
*/

void Editor::BadPixel::enable_dump(unsigned max_dump)
 {
 _dump     = true;
 _max_dump = max_dump;
 }

/*
** ++
**
**
** --
*/

void Editor::BadPixel::_dump_stripe(const Stripe& stripe, unsigned xor_mask, bool flagged) const
 {

 for(unsigned seg = 0; seg < (unsigned)DAQ::Sensor::Segment::NUMOF; seg++)
  {
  int32_t value = stripe.segment[seg] ^ xor_mask;
  bool bad = (value < _low || value > _high);
  if(bad && flagged)
   printf(" *%d*", value);
  else
   printf(" %d", value);
  }

 printf("\n");
 return;
 }

/*
** ++
**
**
** --
*/

static const char STRIPE_HDR[]  = "--- %s CCD %u sample %llu (%u bad segments) ---\n";
static const char STRIPE_CTX[]  = "  [%llu]";
static const char STRIPE_BAD[]  = "> [%llu]";

void Editor::BadPixel::_count(const Stripe* stripes, uint64_t samples, const DAQ::Location& loc, unsigned xor_mask, unsigned ccd, uint64_t sample_base)
 {

 unsigned idx = loc.index();

 bool prev_was_bad = false;

 for(uint64_t s = 0; s < samples; s++)
  {

  const Stripe& stripe = stripes[s];

  unsigned bad_segs = 0;

  for(unsigned seg = 0; seg < (unsigned)DAQ::Sensor::Segment::NUMOF; seg++)
   {
   int32_t value = stripe.segment[seg] ^ xor_mask;
   if(value < _low || value > _high) {_bad[idx]++; bad_segs++;}
   _total[idx]++;
   }
  
  if(bad_segs) _bad_stripes++;

  if(_dump && bad_segs)
   {

   bool can_print = !_max_dump || _dumped < _max_dump;

   if(can_print)
    {

    char locbuf[8];
    uint64_t abs_sample = sample_base + s;

    printf(STRIPE_HDR, loc.encode(locbuf), ccd,
           (unsigned long long)abs_sample, bad_segs);

    if(s > 0 && !prev_was_bad)
     {
     printf(STRIPE_CTX, (unsigned long long)(abs_sample - 1));
     _dump_stripe(stripes[s - 1], xor_mask, false);
     }

    printf(STRIPE_BAD, (unsigned long long)abs_sample);
    _dump_stripe(stripe, xor_mask, true);

    if(s + 1 < samples)
     {
     // Peek ahead: only print trailing context if next stripe is good
     unsigned next_bad = 0;
     const Stripe& next = stripes[s + 1];
     for(unsigned seg = 0; seg < (unsigned)DAQ::Sensor::Segment::NUMOF; seg++)
      {
      int32_t value = next.segment[seg] ^ xor_mask;
      if(value < _low || value > _high) next_bad++;
      }
     if(!next_bad)
      {
      printf(STRIPE_CTX, (unsigned long long)(abs_sample + 1));
      _dump_stripe(next, xor_mask, false);
      }
     }

    _dumped++;
    }

   }
  
  prev_was_bad = (bad_segs > 0);
  }
  
 return;
 }

/*
** ++
**
**
** --
*/

#define SCIENCE_SIZE Science::Data::bytes(SAMPLES)

void Editor::BadPixel::process(Science::Source& source, uint64_t size, uint64_t base)   
 {

 char buffer[8];

 _locs.insert(source.location());

 uint64_t remaining = size;
 uint64_t offset    = 0;

 while(remaining)
  {
  
  uint64_t length = remaining > SCIENCE_SIZE ? SCIENCE_SIZE : remaining; 
  int32_t  error  = _science.read(source, length, offset);
    
   if(!error)
    {
    uint64_t samples     = Science::Data::samples(length);
    uint64_t sample_base = Science::Data::samples(offset);
    unsigned xor_mask = DAQ::Sensor::xor_mask(DAQ::Sensor::SCIENCE);
    _count(_science.ccd(0), samples, source.location(), xor_mask, 0, sample_base);
    _count(_science.ccd(1), samples, source.location(), xor_mask, 1, sample_base);
    _count(_science.ccd(2), samples, source.location(), xor_mask, 2, sample_base);
    }
  else
   printf(ERROR, source.location().encode(buffer), Exception::decode(error));

  remaining -= length;
  offset    += length;
  }
  
 if(source.counters().read.retries || source.counters().read.timouts)
   source.counters().read.dump();

 return;
 } 
  
/*
** ++
**
**
** --
*/

#define GUIDING_SIZE Guiding::Data::bytes(SAMPLES)

void Editor::BadPixel::process(Guiding::Source& source, uint64_t size, uint64_t base)  
 {

 char buffer[8];

 _locs.insert(source.location());

 uint64_t remaining = size;
 uint64_t offset    = 0;

 while(remaining)
  {
  
  uint64_t length = remaining > GUIDING_SIZE ? GUIDING_SIZE : remaining; 
  int32_t  error  = _guiding.read(source, length, offset);
    
   if(!error)
    {
    uint64_t samples     = Guiding::Data::samples(length);
    uint64_t sample_base = Guiding::Data::samples(offset);
    unsigned xor_mask = DAQ::Sensor::xor_mask(DAQ::Sensor::GUIDE);
    _count(_guiding.ccd(0), samples, source.location(), xor_mask, 0, sample_base);
    _count(_guiding.ccd(1), samples, source.location(), xor_mask, 1, sample_base);
    }
  else
    printf(ERROR, source.location().encode(buffer), Exception::decode(error));
 
  remaining -= length;
  offset    += length;
  }
     
 if(source.counters().read.retries || source.counters().read.timouts)
   source.counters().read.dump();

 return;
 } 
 
/*
** ++
**
**
** --
*/

#define WAVEFRONT_SIZE Wavefront::Data::bytes(SAMPLES)

void Editor::BadPixel::process(Wavefront::Source& source, uint64_t size, uint64_t base)  
 {

 char buffer[8];

 _locs.insert(source.location());

 uint64_t remaining = size;
 uint64_t offset    = 0;

 while(remaining)
  {
  
  uint64_t length = remaining > WAVEFRONT_SIZE ? WAVEFRONT_SIZE : remaining;   
  int32_t  error  = _wavefront.read(source, length, offset);
    
   if(!error)
    {
    uint64_t samples     = Wavefront::Data::samples(length);
    uint64_t sample_base = Wavefront::Data::samples(offset);
    unsigned xor_mask = DAQ::Sensor::xor_mask(DAQ::Sensor::WAVEFRONT);
    _count(_wavefront.ccd(0), samples, source.location(), xor_mask, 0, sample_base);
    }
  else
    printf(ERROR, source.location().encode(buffer), Exception::decode(error));
 
  remaining -= length;
  offset    += length;
  }
   
 if(source.counters().read.retries || source.counters().read.timouts)
   source.counters().read.dump();

 return;
 } 

/*
** ++
**
**
** --
*/

uint64_t Editor::BadPixel::bad() const
 {
 uint64_t b = 0;
 DAQ::LocationSet remaining = _locs;
 DAQ::Location loc;
 while(remaining.remove(loc)) b += bad(loc);
 return b;
 }

/*
** ++
**
**
** --
*/

uint64_t Editor::BadPixel::total() const
 {
 uint64_t t = 0;
 DAQ::LocationSet remaining = _locs;
 DAQ::Location loc;
 while(remaining.remove(loc)) t += total(loc);
 return t;
 }

/*
** ++
**
**
** --
*/

static const char  SUMMARY[] = "%*s%s - %016llX - ALL %12llu / %12llu bad pixels\n";
static const char* SPACE = "";

void Editor::BadPixel::dump(int indent) const
 {
 DAQ::LocationSet locs = _locs;
 bool summary = (locs.numof() > 1);
 DAQ::Location loc;
 while(locs.remove(loc)) dump(loc, indent);
 if(summary) dump(DAQ::Location::UNDEFINED, indent);
 }

/*
** ++
**
**
** --
*/

static const char BADPIX[] = "%*s%s - %016llX - %4s - %9llu / %9llu bad pixels\n";

void Editor::BadPixel::dump(const DAQ::Location& loc, int indent) const
 {

 if(DAQ::Location::UNDEFINED == loc.index())
  {
  printf(SUMMARY, indent, SPACE, _image.metadata().timestamp().decode(),
         (unsigned long long)_image.id().value(),
         (unsigned long long)bad(),
         (unsigned long long)total());
  }
 else
  {
  char locbuf[8];
  if(!total(loc)) return;
  printf(BADPIX, indent, SPACE, _image.metadata().timestamp().decode(),
         (unsigned long long)_image.id().value(), loc.encode(locbuf),
         (unsigned long long)bad(loc),
         (unsigned long long)total(loc));
  }
 }


