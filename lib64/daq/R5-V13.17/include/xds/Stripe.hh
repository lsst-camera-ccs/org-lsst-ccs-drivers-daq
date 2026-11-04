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
 
#ifndef XDS_STRIPE
#define XDS_STRIPE

#include "xds/Pagelet.hh"

namespace XDS {

class __attribute__((__packed__)) Stripe {
public:
  enum {LENGTH = 1};                         // Stripe size in units of Pagelets...
  enum {SIZE   = LENGTH * Pagelet::SIZE};    // Stripe Size in units of bytes...
  enum {SECTORS = LENGTH * Pagelet::LENGTH}; // Strip size in units of sectors...
public:
  Stripe() {}
  Stripe(const Stripe&) {}
public:
 ~Stripe() {}
};

}
 
#endif

