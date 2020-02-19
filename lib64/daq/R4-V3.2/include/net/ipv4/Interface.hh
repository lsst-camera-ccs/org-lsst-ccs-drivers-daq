
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
 
#ifndef IPV4_INTERFACE
#define IPV4_INTERFACE

#include <stdint.h>

#include "net/ipv4/Address.hh"
#include "net/ipv4/Flags.hh"

namespace IPV4 {

class Interface {
public:
  Interface() {}
public:
  Interface(const char* name, const Address&, uint32_t bcst, uint32_t mask, unsigned flags, unsigned mtu);
public:   
  Interface(const Interface&);
public:
  Interface& operator=(const Interface&);
public:  
 ~Interface() {}
public: 
  const char* name() const {return _name;} 
public:
  const Address& address() const {return _address;}
public:    
  unsigned flags()  const {return _flags;}
  uint32_t bcst()   const {return _bcst;}
  uint32_t mask()   const {return _mask;}                             
  uint32_t subnet() const {return _mask & (uint32_t)_address;} 
  unsigned mtu()    const {return _mtu;}                                          
public:
  void print(int indent=0) const; 
private:
  char        _name[16];
  Address     _address;
  uint32_t    _bcst;
  uint32_t    _mask;                               
  unsigned    _flags;
  unsigned    _mtu;
  };
 
}

#endif

