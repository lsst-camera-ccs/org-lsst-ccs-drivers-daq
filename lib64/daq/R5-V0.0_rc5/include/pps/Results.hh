
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
 
#ifndef PPS_RESULTS
#define PPS_RESULTS

#include <stdint.h>

#include "dsi/Location.hh"

namespace PPS {

class Test; // forward link...

class  __attribute__((__packed__)) Results {
public:
  enum {MAX=8*1024};
  enum {MASK=MAX-1};
public:
  static const char*  banner();
public:
  Results() {}
  Results(const DSI::Location& location, uint32_t size, uint32_t rescale);
public:
  Results(const Results&) = delete;
public:
 ~Results() {}
public:
  uint32_t loops() const;
  uint32_t size()  const;
public:
  const DSI::Location& location() const {return _location;}
public: 
  uint32_t* element(uint32_t loops) {return &_samples[loops & MASK];}
public:  
  float mu()              const; // in RCE clocks tics (1 tic = 3.2 ns) 
  float sigma(float mean) const; // in RCE clocks tics (1 tic = 3.2 ns) 
public:
  void dump(int indent=0) const;
public:
  float rescale(float tics) const; // tics to NS... 
private:
  friend class Test;
private:  
  void _fixup(uint32_t loops);
private:
  DSI::Location _location; 
private:  
  uint8_t  _pad[3];
  uint32_t _loops;
  uint32_t _size;
  uint32_t _rescale;
  uint32_t _samples[MAX];
  };
 
}

#endif


