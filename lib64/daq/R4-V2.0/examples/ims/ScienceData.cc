
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

#include <stdio.h>

#include "ims/science/Data.hh"

#include "ScienceData.hh"

using namespace IMS;

/*
** ++
**
**
** --
*/

ScienceData::ScienceData(unsigned samples) : 
  Data(),
  _buffer(new char[Science::Data::bytes(samples)]),
  _opaque(_buffer, samples),
  _ccds(new Stripe[samples * 3])
  { 
  
  _ccd[0] = _ccds;
  _ccd[1] = _ccds + samples;
  _ccd[2] = _ccds + samples + samples;
       
  }

/*
** ++
**
**
** --
*/

int32_t ScienceData::read(Science::Source& source, uint64_t length)
 {
 
 int32_t error = _opaque.read(source, length);
 
 if(!error) _opaque.decode012(_ccd[0], _ccd[1], _ccd[2], length);
 
 return error;
 }

/*
** ++
**
**
** --
*/

uint64_t ScienceData::compare(const DAQ::Location& location, const ScienceData& data, uint64_t size)
 {
 
 uint64_t samples = Science::Data::samples(size);
 
 uint64_t matched;
 
 matched  = _compare(_ccd[0], data._ccd[0], samples, location, 0); 
 matched += _compare(_ccd[1], data._ccd[1], samples, location, 1); 
 matched += _compare(_ccd[2], data._ccd[2], samples, location, 2); 

 uint64_t bytes = matched * sizeof(Sample);
  
 return bytes;
 }


  


