
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
 
#ifndef IMS_SAMPLE
#define IMS_SAMPLE

#include <stdint.h>

#include "daq/Sensor.hh"
#include "ims/Stripe.hh"

namespace IMS {

class Sample {
public:
  Sample() {}     
public:
 ~Sample() {}
public:
  Sample*       encode(const Stripe* stripe, DAQ::Sensor::Type type=DAQ::Sensor::Type::GUIDE)       {_encode(stripe); return &this[(int)type];} 
  const Sample* decode(      Stripe* stripe, DAQ::Sensor::Type type=DAQ::Sensor::Type::GUIDE) const {_decode(stripe); return &this[(int)type];} 
private:
  void _encode(const Stripe*);
  void _decode(      Stripe*) const; 
private:
  uint8_t _sample[(int)DAQ::Sensor::Segment::SIZEOF]; 
 };

}

#endif

