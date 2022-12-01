
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
 
#ifndef UDP_MEMBERS
#define UDP_MEMBERS

#include "dvi/Endian.hh"

namespace UDP  {

class __attribute__((__packed__)) Members {
public:
  Members() {}
  Members(uint16_t dst, uint16_t src, uint16_t length) : _src(src), _dst(dst), _length(DVI::Endian::wire((uint16_t)(length + sizeof(Members)))), _checksum(0) {} 
public:  
  Members(const Members& clone) {uint64_t* dst = (uint64_t*)this; uint64_t* src = (uint64_t*)&clone; dst[0] = src[0];}   
public:
 ~Members() {}
public:
  uint16_t length() const {return DVI::Endian::local(_length);} 
  uint16_t src()    const {return DVI::Endian::local(_src);}
  uint16_t dst()    const {return DVI::Endian::local(_dst);}
private:
  uint16_t _src;
  uint16_t _dst;
  uint16_t _length;
  uint16_t _checksum;
  };
 
}

#endif


