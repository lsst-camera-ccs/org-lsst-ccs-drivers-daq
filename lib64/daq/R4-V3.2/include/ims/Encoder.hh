
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
 
#ifndef IMS_ENCODER
#define IMS_ENCODER

#include "daq/LocationSet.hh"
#include "ims/Image.hh"
#include "ims/science/Source.hh"
#include "ims/guiding/Source.hh"
#include "ims/wavefront/Source.hh"

namespace IMS {

class Encoder {   
public:
  Encoder() = delete;
public:  
  Encoder(Image&);
  Encoder(Image&,   const DAQ::LocationSet& filter);
  Encoder(Encoder&, const DAQ::LocationSet& filter);
  Encoder(Encoder&);
public:
  virtual ~Encoder() {}
public:  
  Encoder& operator=(const Encoder&);
public:
  void run();
public:
  virtual void process(Science::Source&)   = 0;
  virtual void process(Guiding::Source&)   = 0;
  virtual void process(Wavefront::Source&) = 0;
protected:
  Image& _image;
private:  
  void _science();
  void _guiding();
  void _wavefront();  
private:
  DAQ::LocationSet _elements;
};
 
}

#endif

