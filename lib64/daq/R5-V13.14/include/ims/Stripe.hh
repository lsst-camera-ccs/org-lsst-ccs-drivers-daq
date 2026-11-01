
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
 
#ifndef IMS_STRIPE
#define IMS_STRIPE

#include <stdint.h>

#include "daq/Sensor.hh"

namespace IMS {

class Stripe {
public:
  Stripe() {}
  Stripe(const Stripe&);     
public:
 ~Stripe() {}
public:  
  Stripe& operator=(const Stripe&);
public: 
  bool operator==(const Stripe&) const;
  bool operator!=(const Stripe&) const;
public:
  void dump(int indent=0) const;   
public:
  int32_t segment[(unsigned)DAQ::Sensor::Segment::NUMOF]; 
 };

}

#endif

