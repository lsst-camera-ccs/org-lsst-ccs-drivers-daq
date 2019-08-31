
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
 
#ifndef IMS_GUIDINGDATA
#define IMS_GUIDINGDATA

#include "daq/Location.hh"
#include "ims/Stripe.hh"
#include "ims/guiding/Source.hh"
#include "ims/guiding/Data.hh"
#include "Data.hh"

namespace IMS {
 
class GuidingData  : public Data {
public:
  GuidingData()             = delete; 
  GuidingData(GuidingData&) = delete; 
public:
  GuidingData(unsigned samples);
public:
 ~GuidingData() {delete[] _buffer; delete[] _ccds;}
public:
  int32_t read(Guiding::Source&, uint64_t length);
public:   
  uint64_t compare(const DAQ::Location&, const GuidingData&, uint64_t length);
private:
  char*         _buffer; 
  Guiding::Data _opaque;     
  Stripe*       _ccds; 
  Stripe*       _ccd[2];     
};

}

#endif

