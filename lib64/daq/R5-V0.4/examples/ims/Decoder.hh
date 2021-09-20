
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
 
#ifndef IMS_EDITOR_DECODER
#define IMS_EDITOR_DECODER

#include <stdint.h>

#include "ims/Decoder.hh"
#include "ims/Image.hh"

#include "ScienceData.hh"
#include "GuidingData.hh"
#include "WavefrontData.hh"

namespace IMS {namespace Editor {

class Decoder : public IMS::Decoder {
public:
  Decoder()               = delete; 
  Decoder(const Decoder&) = delete;  
public:
  Decoder(Image&);
public:
 ~Decoder() {} 
public:
  void process(Science::Source&,   uint64_t length, uint64_t base);
  void process(Guiding::Source&,   uint64_t length, uint64_t base);
  void process(Wavefront::Source&, uint64_t length, uint64_t base);
public:
  uint64_t size() const {return _size;}                                
private:
  uint64_t      _size;
  ScienceData   _science;
  GuidingData   _guiding;
  WavefrontData _wavefront;
};
 
}}

#endif

