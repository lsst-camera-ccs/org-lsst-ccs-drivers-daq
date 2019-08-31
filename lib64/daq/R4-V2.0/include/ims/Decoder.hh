
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
 
#ifndef IMS_DECODER
#define IMS_DECODER

#include "daq/LocationSet.hh"
#include "ims/Image.hh"
#include "ims/science/Source.hh"
#include "ims/guiding/Source.hh"
#include "ims/wavefront/Source.hh"

namespace IMS {

class Decoder {   
public:
  Decoder() = delete;
public:  
  Decoder(Image&);
  Decoder(Image&,   const DAQ::LocationSet& filter);
  Decoder(Decoder&, const DAQ::LocationSet& filter);
  Decoder(Decoder&);
public:
  virtual ~Decoder() {}
public:  
  Decoder& operator=(const Decoder&);
public:
  void run();
public:
  virtual void process(Science::Source&,   uint64_t length, uint64_t offset) = 0;
  virtual void process(Guiding::Source&,   uint64_t length, uint64_t offset) = 0;
  virtual void process(Wavefront::Source&, uint64_t length, uint64_t offset) = 0;
protected:
  Image& _image;
private:  
  void _science();
  void _guiding();
  void _wavefront();   
  void _wait(Stream*);
private:
  DAQ::LocationSet _elements;
};
 
}

#endif

