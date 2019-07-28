
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

#include "net/Endian.hh"

namespace XDS {

class Stat {
public: 
  Stat()                 : _stat(uint64_t(0))             {}
  Stat(uint64_t stat)    : _stat(Net::Endian::wire(stat)) {}
  Stat(const Stat& stat) : _stat(stat._stat)              {}
public:
 ~Stat() {} 
public: 
  uint64_t decode() const {return Net::Endian::local(_stat);}
public: 
  Stat& operator=(const Stat& clone)  {_stat = clone._stat;              return *this;} 
  Stat& operator=(uint64_t clone)     {_stat = Net::Endian::wire(clone); return *this;}
public:  
  Stat& operator+=(const Stat& clone) {uint64_t x = decode(); uint64_t y = clone.decode(); x += y; _stat = Net::Endian::wire(x); return *this;}
public:  
  Stat& operator++()    {uint64_t x = decode(); _stat = Net::Endian::wire(x + 1); return *this;}   
  Stat  operator++(int) {uint64_t x = decode(); _stat = Net::Endian::wire(x + 1); return x;}
private:
  uint64_t _stat; 
  };

}

#endif

