
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

int32_t ScienceData::read(Science::Source& source, uint64_t length, uint64_t offset)
 {
   //char buf[16];
   //printf("IMS::Editor:ScienceData::read() - Source %s, legnth %lli, offset %lli\n", source.location().encode(buf), (unsigned long long)length, (unsigned long long)offset);

 static int32_t location=DAQ::Location::UNDEFINED;
 static int32_t current=1;

 if(source.location().index() != location) 
 {
   location = source.location().index();
   current = 1;
 }
 
 int32_t error = _opaque.read(source, length, offset);
 
 if(!error) _opaque.decode012(_ccd[0], _ccd[1], _ccd[2], length);
 
//// Stripe* ccd0 = _ccd[0];
//// Stripe* ccd1 = _ccd[1];
//// Stripe* ccd2 = _ccd[2];
//// 
//// for(int sample=0; Science::Data::bytes(sample)<length; ++sample)
//// {
////   for(int segment=0; segment<(int)DAQ::Sensor::Segment::NUMOF; ++segment)
////   {
////     if(ccd2->segment[segment] != (0x3FFFF&current))
///////       printf("*");
///////     else
///////       printf(" ");
////       printf("sample %i, ccd %i, segment %2i, data %X, current %X\n",
////	      sample, 2, segment, ccd2->segment[segment], current-1);
///////       printf("sample %i, ccd %i, segment %2i, data %X, current %X %X\n",
///////	      sample, 2, segment, ccd2->segment[segment], 0x3FFFF&current, current);
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
////   ++ccd0; ++ccd1; ++ccd2;
////
//// }

// for(int i=0; i<1; ++i) 
// {
//   printf("ccd[0]:\n");
//   _ccd[0]->dump(4);   
//
//   printf("ccd[1]:\n");
//   _ccd[1]->dump(4);   
//
//   printf("ccd[2]:\n");
//   _ccd[2]->dump(4);   
// }
 
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


  


