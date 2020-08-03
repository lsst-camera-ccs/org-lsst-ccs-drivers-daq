
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

#include "ims/Decoder.hh"

namespace IMS {namespace Editor {

class Inspector : public Decoder {
public:
  Inspector()                 = delete; 
  Inspector(const Inspector&) = delete;  
public:
  Inspector(Image&);
public:
 ~Inspector() {} 
public:
  void process(Science::Source&,   uint64_t length, uint64_t base);
  void process(Guiding::Source&,   uint64_t length, uint64_t base);
  void process(Wavefront::Source&, uint64_t length, uint64_t base);
public:
  void summary() const;                               
private:
  uint64_t _size;
  unsigned _total;
};
 
}}

#endif

