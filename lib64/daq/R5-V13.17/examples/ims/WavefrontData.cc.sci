
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

int32_t WavefrontData::read(Wavefront::Source& source, uint64_t length, uint64_t offset)
 {
 
   //char buf[16];
   //bprintf("IMS::Editor:ScienceData::read() - Source %s, legnth %lli, offset %lli\n", source.location().encode(buf), (unsigned long long)length, (unsigned long long)offset);

// static int32_t location=DAQ::Location::UNDEFINED;
// static int32_t current=1;
//
// if(source.location().index() != location) 
// {
//   location = source.location().index();
//   current = 1;
// }

 int32_t error = _opaque.read(source, length, offset);
 
 if(!error) _opaque.decode0(_ccd[0], length);

// Stripe* ccd0 = _ccd[0];
//
// for(int sample=0; Wavefront::Data::bytes(sample)<length; ++sample)
// {
//   for(int segment=0; segment<(int)DAQ::Sensor::Segment::NUMOF; ++segment)
//   {
//     if(ccd0->segment[segment] != (0x3FFFF&current))
//       printf("sample %i, ccd %i, segment %2i, data %X, current %X\n",
//	      sample, 0, segment, ccd0->segment[segment], current);
//     ++current;
//   }
//   ++ccd0;
// }
 
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
 
 
