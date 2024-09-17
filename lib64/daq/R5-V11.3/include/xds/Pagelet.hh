/*
** ++
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
**	000 - January 09, 2007
**
**  Revision History:
**	None.
**
** --
*/
 
#ifndef XDS_PAGELET
#define XDS_PAGELET

#include "xds/Sector.hh"

namespace XDS {

class Pagelet {
public:
  enum {LENGTH = 127};                   // Size in units of sectors). Note: This value is set to be as close to, but under the maximum DSI datagram size (less protocol overhead).
  enum {SIZE   = LENGTH * Sector::SIZE}; // Size in units of bytes...
public: 
  Pagelet()                     {}
  Pagelet(const Pagelet& clone) {}
public:
 ~Pagelet() {}
  };

}
 
#endif

