
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
 
#ifndef IMS_WAVEFRONT_DATA
#define IMS_WAVEFRONT_DATA

#include <stdint.h>

#include "daq/Sensor.hh"
#include "ims/Sample.hh"
#include "ims/wavefront/Source.hh"

namespace IMS { namespace Wavefront {
 
class Data  {
public:
  enum : uint64_t {SIZEOF_SAMPLE = DAQ::Sensor::Type::WAVEFRONT * sizeof(IMS::Sample)};
public:
  static uint64_t bytes(  uint64_t samples) {return samples * SIZEOF_SAMPLE;}
  static uint64_t samples(uint64_t bytes)   {return bytes   / SIZEOF_SAMPLE;}
public:
  Data()            = delete; 
  Data(const Data&) = delete;
public:                      
  Data(char* buffer, uint64_t samples) : _data(buffer), _size(bytes(samples)) {} 
public:
  ~Data() {}
public:
  uint64_t size() const {return _size;} 
public: 
  int32_t read(Wavefront::Source& source)                                   {return source.read(_data, _size);} 
  int32_t read(Wavefront::Source& source, uint64_t length)                  {return source.read(_data, length);}
  int32_t read(Wavefront::Source& source, uint64_t length, uint64_t offset) {return source.read(_data, length, offset);}
public:
  int32_t write(Wavefront::Source& source)                  {return source.write(_data, _size);}
  int32_t write(Wavefront::Source& source, uint64_t length) {return source.write(_data, length);}
public:
 void decode0(  Stripe ccd0[], uint64_t length);
public:
 void decode0(  Stripe ccd0[]) {decode0(ccd0, _size);}
public: 
  void encode(const Stripe ccd0[], uint64_t length);
  void encode(const Stripe ccd0[]) {encode(ccd0, _size);}
public: 
  char*    _data;   
  uint64_t _size;
};

}}

#endif

