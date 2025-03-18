
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
 
#ifndef EMU_GENERATOR
#define EMU_GENERATOR

#include <stdint.h>

#include "ims/Encoder.hh"
#include "ims/Image.hh"
#include "ims/science/Source.hh"
#include "ims/guiding/Source.hh"
#include "ims/wavefront/Source.hh"
#include "Random.hh"

namespace EMU {

class Generator : public IMS::Encoder {
public:
  Generator()                 = delete; 
  Generator(const Generator&) = delete;  
public:
  Generator(IMS::Image&);
public:
 ~Generator() {} 
public:
  void process(IMS::Science::Source&);
  void process(IMS::Guiding::Source&);
  void process(IMS::Wavefront::Source&);
public:
  uint64_t size() const {return _size;}                                
private:
  Random   _data;
  uint64_t _size;
};
 
}

#endif



