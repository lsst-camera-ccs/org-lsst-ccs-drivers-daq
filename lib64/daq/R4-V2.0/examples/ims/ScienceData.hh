
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
 
#ifndef IMS_SCIENCEDATA
#define IMS_SCIENCEDATA

#include "daq/Location.hh"
#include "ims/Stripe.hh"
#include "ims/science/Source.hh"
#include "ims/science/Data.hh"
#include "Data.hh"

namespace IMS {
 
class ScienceData  : public Data {
public:
  ScienceData()             = delete; 
  ScienceData(ScienceData&) = delete; 
public:
  ScienceData(unsigned samples);
public:
 ~ScienceData() {delete[] _buffer; delete[] _ccds;}
public:
  int32_t read(Science::Source&, uint64_t length);
public:   
  uint64_t compare(const DAQ::Location&, const ScienceData&, uint64_t length);
private:
  char*         _buffer; 
  Science::Data _opaque;     
  Stripe*       _ccds; 
  Stripe*       _ccd[3];     
};

}

#endif

