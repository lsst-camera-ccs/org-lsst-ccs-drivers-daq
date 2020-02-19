
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

#include "ims/wavefront/Data.hh"

#include "WavefrontData.hh"

using namespace IMS;

/*
** ++
**
**
** --
*/

WavefrontData::WavefrontData(unsigned samples) : 
  Data(),
  _buffer(new char[Wavefront::Data::bytes(samples)]),
  _opaque(_buffer, samples),
  _ccds(new Stripe[samples])
  { 
  
  _ccd[0] = _ccds;
  
  }

/*
** ++
**
**
** --
*/

int32_t WavefrontData::read(Wavefront::Source& source, uint64_t length)
 {
 
 int32_t error = _opaque.read(source, length);
 
 if(!error) _opaque.decode0(_ccd[0], length);
 
 return error;
 }

/*
** ++
**
**
** --
*/

uint64_t WavefrontData::compare(const DAQ::Location& location, const WavefrontData& data, uint64_t size)
 {
 
 uint64_t samples = Wavefront::Data::samples(size);
 uint64_t matched = _compare(_ccd[0], data._ccd[0], samples, location, 0); 

 uint64_t bytes = matched * sizeof(IMS::Sample);
 
 return bytes;  
 }
 
 