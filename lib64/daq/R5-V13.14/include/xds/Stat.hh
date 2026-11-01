
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
 
#ifndef XDS_STAT
#define XDS_STAT

#include "dvi/Endian.hh"

namespace XDS {

class __attribute__((__packed__)) Stat {
public: 
  Stat()                 : _stat(uint32_t(0))             {}
  Stat(uint32_t stat)    : _stat(DVI::Endian::wire(stat)) {}
  Stat(const Stat& stat) : _stat(stat._stat)              {}
public:
 ~Stat() {} 
public: 
  uint32_t decode() const {return DVI::Endian::local(_stat);}
public: 
  Stat& operator=(const Stat& clone)  {_stat = clone._stat;              return *this;} 
  Stat& operator=(uint32_t clone)     {_stat = DVI::Endian::wire(clone); return *this;}
public:  
  Stat& operator+=(const Stat& clone) {uint32_t x = decode(); uint32_t y = clone.decode(); x += y; _stat = DVI::Endian::wire(x); return *this;}
public:  
  Stat& operator++()    {uint32_t x = decode(); _stat = DVI::Endian::wire(x + 1); return *this;}   
  Stat  operator++(int) {uint32_t x = decode(); _stat = DVI::Endian::wire(x + 1); return x;}
private:
  uint32_t _stat; 
  };

}

#endif

