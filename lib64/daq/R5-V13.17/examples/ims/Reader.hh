
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
 
#ifndef IMS_EDITOR_READER
#define IMS_EDITOR_READER

#include <stdint.h>

#include "ims/Decoder.hh"
#include "ims/Image.hh"

#include "ims/science/Data.hh"
#include "ims/guiding/Data.hh"
#include "ims/wavefront/Data.hh"

namespace IMS {namespace Editor {

class Reader : public IMS::Decoder {
public:
  Reader()              = delete; 
  Reader(const Reader&) = delete;  
public:
  Reader(Image&);
  Reader(Image&, const DAQ::LocationSet& filter);
public:
 ~Reader() {delete[] _buffer;} 
public:
  void process(Science::Source&,   uint64_t length, uint64_t base);
  void process(Guiding::Source&,   uint64_t length, uint64_t base);
  void process(Wavefront::Source&, uint64_t length, uint64_t base);
public:
  uint64_t processed() const {return _processed;}                                
private:
  uint64_t        _processed;
  char*           _buffer; 
  Science::Data   _science;
  Guiding::Data   _guiding; 
  Wavefront::Data _wavefront;      
};
 
}}

#endif

