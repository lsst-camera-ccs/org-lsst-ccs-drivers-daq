
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

int32_t GuidingData::read(Guiding::Source& source, uint64_t length, uint64_t offset)
 {
 
   //char buf[16];
   //printf("IMS::Editor:GuidingData::read() - Source %s, legnth %lli, offset %lli\n", source.location().encode(buf), (unsigned long long)length, (unsigned long long)offset);

 static int32_t location=DAQ::Location::UNDEFINED;
 static int32_t current=1;

 if(source.location().index() != location) 
 {
   location = source.location().index();
   current = 1;
 }

 int32_t error = _opaque.read(source, length, offset);
 
 if(!error) _opaque.decode01(_ccd[0], _ccd[1], length);
 
//// Stripe* ccd0 = _ccd[0];
//// Stripe* ccd1 = _ccd[1];
////
//// for(int sample=0; Guiding::Data::bytes(sample)<length; ++sample)
//// {
////
////   for(int segment=0; segment<(int)DAQ::Sensor::Segment::NUMOF; ++segment)
////   {
////     if(ccd0->segment[segment] != (0x3ffff&current))
///////       printf("*");
///////     else
///////       printf(" ");
////       printf("sample %i, ccd %i, segment %2i, data %X, current %X\n",
////	      sample, 0, segment, ccd0->segment[segment], current-1);
///////       printf("sample %i, ccd %i, segment %2i, data %X, current %X %X\n",
///////	      sample, 0, segment, ccd0->segment[segment], 0x3FFFF&current, current);
////       ++current;
////   }
////
////   for(int segment=0; segment<(int)DAQ::Sensor::Segment::NUMOF; ++segment)
////   {
////     if(ccd1->segment[segment] != (0x3ffff&current))
///////       printf("*");
///////     else
///////       printf(" ");
////       printf("sample %i, ccd %i, segment %2i, data %X, current %X\n",
////	      sample, 1, segment, ccd1->segment[segment], current-1);
///////       printf("sample %i, ccd %i, segment %2i, data %X, current %X %X\n",
///////	      sample, 1, segment, ccd1->segment[segment], 0x3FFFF&current, current);
////       ++current;
////   }
////
////   ++ccd0; ++ccd1;
////
//// }

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



