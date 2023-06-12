
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
 
#ifndef IPV4_INTERFACES
#define IPV4_INTERFACES

#include <sys/types.h>
#include <ifaddrs.h>
       
#include "net/ipv4/Flags.hh"
#include "net/ipv4/Interface.hh"
#include "net/ipv4/Address.hh"

namespace IPV4 {

class Interfaces {
public:
  Interfaces(unsigned accepts=Flags::ALL, unsigned rejects=Flags::NONE);
public:  
 ~Interfaces();
public:
  const Interface* remove(Interface&);
  const Interface* remove(Interface&, const char* name); 
  const Interface* remove(Interface&, uint32_t address);
public:
   const Interface* remove192_168(Interface&);
   const Interface* remove172_016(Interface&);
   const Interface* remove010_000(Interface&);
public:
  int numof(); 
private:
  bool _reject(struct ::ifaddrs*, unsigned accepts, unsigned rejects);
private:
  int               _fd;
  struct ::ifaddrs* _interfaces;
  struct ::ifaddrs* _l3;
  struct ::ifaddrs  _l2; 
  };
 
}

#endif

