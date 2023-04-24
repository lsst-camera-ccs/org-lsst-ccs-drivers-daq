
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
 
#ifndef IMS_DATA
#define IMS_DATA

#include "daq/Location.hh"
#include "ims/Stripe.hh"

namespace IMS {
 
class Data  {
public:
  Data(Data&) = delete; 
public:
  Data() {}
public:
 ~Data() {}
protected:
  uint64_t _compare(const Stripe A[], const Stripe B[], uint64_t samples, const DAQ::Location&, int ccd);
};

}

#endif

