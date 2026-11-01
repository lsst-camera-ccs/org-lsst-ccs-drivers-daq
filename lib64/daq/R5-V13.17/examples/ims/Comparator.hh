
/*
**  Package:
**	
**
**  Abstract:
**      
**
**  Author:
**      Michael Huffer, SLAC (mehsys@slac.stanford.edu)
**
**  Creation Date:
**	    000 - April 06, 2011
**
**  Revision History:
**	    None.
**
** --
*/
 
#ifndef IMS_COMPARATOR
#define IMS_COMPARATOR

#include <stdint.h>

#include "ims/Decoder.hh"
#include "ims/Image.hh"
#include "ims/Id.hh"

#include "ScienceData.hh"
#include "GuidingData.hh"
#include "WavefrontData.hh"

namespace IMS {namespace Editor {

class Comparator : public Decoder {
public:
  Comparator()                  = delete; 
  Comparator(const Comparator&) = delete;  
public:
  Comparator(Image&, Id&, Store&);
public:
 ~Comparator() {} 
public:
  void process(Science::Source&,   uint64_t length, uint64_t base);
  void process(Guiding::Source&,   uint64_t length, uint64_t base);
  void process(Wavefront::Source&, uint64_t length, uint64_t base);
public:
  uint64_t processed() const {return _processed;} 
  uint64_t matched()   const {return _matched;}                                                              
private:
  Store&        _store;
  Id&           _id;
  uint64_t      _processed;
  uint64_t      _matched;
  ScienceData   _science_a;
  ScienceData   _science_b;
  GuidingData   _guiding_a;
  GuidingData   _guiding_b;
  WavefrontData _wavefront_a;
  WavefrontData _wavefront_b;
};
 
}}

#endif

