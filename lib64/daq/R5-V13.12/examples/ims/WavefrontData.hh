
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
 
#ifndef IMS_WAVEFRONTDATA
#define IMS_WAVEFRONTDATA

#include "daq/Location.hh"
#include "ims/Stripe.hh"
#include "ims/wavefront/Source.hh"
#include "ims/wavefront/Data.hh"
#include "Data.hh"

namespace IMS {
 
class WavefrontData  : public Data {
public:
  WavefrontData()             = delete; 
  WavefrontData(WavefrontData&) = delete; 
public:
  WavefrontData(unsigned samples);
public:
 ~WavefrontData() {delete[] _buffer; delete[] _ccds;}
public:
  int32_t read(Wavefront::Source&, uint64_t length, uint64_t offset=0);
public:   
  uint64_t compare(const DAQ::Location&, const WavefrontData&, uint64_t length);
private:
  char*           _buffer; 
  Wavefront::Data _opaque;     
  Stripe*         _ccds;
  Stripe*         _ccd[1]; 
};

}

#endif

