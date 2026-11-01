
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
 
#ifndef IMS_EDITOR_INSPECTOR
#define IMS_EDITOR_INSPECTOR

#include <stdint.h>

#include "daq/LocationSet.hh"
#include "ims/Decoder.hh"

namespace IMS {namespace Editor {

class Inspector : public Decoder {
public:
  Inspector()                 = delete; 
  Inspector(const Inspector&) = delete;  
public:
  Inspector(Image&, bool verbose=true);
public:
 ~Inspector() {} 
public:
  void process(Science::Source&,   uint64_t length, uint64_t base);
  void process(Guiding::Source&,   uint64_t length, uint64_t base);
  void process(Wavefront::Source&, uint64_t length, uint64_t base);
public:
  void summary() const;
public:
  bool     complete() const {return !_expected;}
  unsigned sources()  const {return _total;}
  uint64_t size()     const {return _size;}
private:
  bool             _verbose;
  uint64_t         _size;
  unsigned         _total;
  DAQ::LocationSet _expected;
};
 
}}

#endif

