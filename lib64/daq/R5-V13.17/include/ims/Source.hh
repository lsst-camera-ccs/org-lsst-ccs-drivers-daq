
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
 
#ifndef IMS_SOURCE
#define IMS_SOURCE

#include <stdint.h>

#include "daq/Sensor.hh"
#include "daq/Location.hh"
#include "ims/Id.hh"
#include "ims/StoreBase.hh"
#include "ims/SourceMetadata.hh"
#include "ims/Bucket.hh"

namespace IMS {

class Source  {
public:
 enum : uint8_t {EOS=1}; // Marker to delineate End-Of-Stream (see the publish method")... 
public:  
  static void title(int indent=0) {return SourceMetadata::title(indent);}
public:
  Source(const Id&, const DAQ::Location&, StoreBase&); // Read...
  Source(const Id&, const DAQ::Location&, StoreBase&, const SourceMetadata&); // Write...
  Source(const Source&);
public:
  Source() = delete;
public:
 ~Source() {}
public:  
  Source& operator=(const Source&);
public:
  operator bool() const;  
public:
  bool science()   const {return _metadata.sensor() == DAQ::Sensor::Type::SCIENCE;} 
  bool guider()    const {return _metadata.sensor() == DAQ::Sensor::Type::GUIDE;} 
  bool wavefront() const {return _metadata.sensor() == DAQ::Sensor::Type::WAVEFRONT;} 
public: 
  const SourceMetadata& metadata() const;
  uint64_t              size();
  const DAQ::Location&  location() const;
public:    
  void synopsis(                 int indent=0) {_synopsis(size(), indent);}    
  void synopsis(uint64_t length, int indent=0) {_synopsis(length, indent);}
public:
  int32_t read(   char*       buffer, uint64_t length, uint64_t offset=0); 
  int32_t write(  const char* buffer, uint64_t length);             
  int32_t publish(const char* buffer, uint64_t length,  uint8_t marker=0);             
public:
  int32_t error() const;
public:
  bool accumulate(uint64_t length) {uint64_t accumulation = _accumulation; accumulation += length; _accumulation = accumulation; return accumulation >= _size;} 
  void set(uint64_t size) {_size = size;}
public:
  const XDS::Counters& bcounters() const {return _bucket.counters();}
  const XDS::Counters&  counters() const {return _data.counters();}
private:    
  void _synopsis(uint64_t length, int indent) const;
private:
  friend class Remover;  
private:
  int32_t _remove();
private:
  Bucket         _bucket;
  Bucket         _data;
  uint64_t       _accumulation;
  uint64_t       _size;
  SourceMetadata _metadata;
  int32_t        _error;
  
  
  DAQ::Location  _location;
};

}

#endif

