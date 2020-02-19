
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

#include "ims/guiding/Data.hh"

#include "GuidingData.hh"

using namespace IMS;

/*
** ++
**
**
** --
*/

GuidingData::GuidingData(unsigned samples) :
  Data(), 
  _buffer(new char[Guiding::Data::bytes(samples)]),
  _opaque(_buffer, samples),
  _ccds(new Stripe[samples * 2])
  { 
  
  _ccd[0] = _ccds;
  _ccd[1] = _ccds + samples;
       
  }

/*
** ++
**
**
** --
*/

int32_t GuidingData::read(Guiding::Source& source, uint64_t length)
 {
 
 int32_t error = _opaque.read(source, length);
 
 if(!error) _opaque.decode01(_ccd[0], _ccd[1], length);
 
 return error;
 }

/*
** ++
**
**
** --
*/

uint64_t GuidingData::compare(const DAQ::Location& location, const GuidingData& data, uint64_t size)
 {
  
 uint64_t samples = Guiding::Data::samples(size);
 
 uint64_t matched;
 
 matched  = _compare(_ccd[0], data._ccd[0], samples, location, 0); 
 matched += _compare(_ccd[1], data._ccd[1], samples, location, 1); 
 
 uint64_t bytes = matched * sizeof(Sample);
  
 return bytes;
 }



