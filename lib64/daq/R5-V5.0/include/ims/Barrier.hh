
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
 
#ifndef IMS_BARRIER
#define IMS_BARRIER

#include "daq/LocationSet.hh"
#include "ims/Store.hh"
#include "ims/Image.hh"
#include "ims/Decoder.hh"
#include "ims/science/Source.hh"
#include "ims/guiding/Source.hh"
#include "ims/wavefront/Source.hh"

namespace IMS {

class Barrier : public Decoder {
public:
  Barrier(Image& image)                                 : Decoder(image)         {}
  Barrier(Image& image, const DAQ::LocationSet& filter) : Decoder(image, filter) {}
public:
  Barrier()               = delete;
  Barrier(const Barrier&) = delete;
public:
 ~Barrier() {}
public:
  void block(Stream& stream, unsigned tmo=0) {run(stream, tmo);}
public:
  void process(Science::Source&,   uint64_t length, uint64_t offset) {return;}
  void process(Guiding::Source&,   uint64_t length, uint64_t offset) {return;}
  void process(Wavefront::Source&, uint64_t length, uint64_t offset) {return;}
};
 
}

#endif

