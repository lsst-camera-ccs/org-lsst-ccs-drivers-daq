
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
 
#ifndef PPS_PATTERN
#define PPS_PATTERN

#include <stdint.h>

namespace PPS { 

class  __attribute__((__packed__)) __attribute__((__aligned__(4))) Pattern {
public:
 enum {MAX=(1024*64)/sizeof(uint16_t)};
public:
  Pattern();
public:
  Pattern(const Pattern&) = delete;
public:
 ~Pattern() {}
public:  
  //const Pattern& operator=(const Command& clone) {*(Command*)this = clone; return *this;}
public:
  void verify(unsigned length, unsigned expected) const;
private:
  uint16_t _vector[MAX];     
  };
 
}

#endif


