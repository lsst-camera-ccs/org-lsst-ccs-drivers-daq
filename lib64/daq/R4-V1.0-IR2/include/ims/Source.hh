
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
#include "xds/Event.hh"
#include "ims/Id.hh"
#include "ims/Store.hh"
#include "ims/SourceMetadata.hh"
#include "ims/Bucket.hh"

namespace IMS {

class Source  {
public:  
  static void title(int indent=0);
public:
  Source(const Source&);
  Source(const Id&, const DAQ::Location&, Store&);
  Source(const Id&, const DAQ::Location&, Store&, uint64_t size);
  Source(const Id&, const DAQ::Location&, Store&, const SourceMetadata&);
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
  uint64_t              size()     const;
  const DAQ::Location&  location() const;
public:    
  void synopsis(                 int indent=0) const {_synopsis(size(), indent);}    
  void synopsis(uint64_t length, int indent=0) const {_synopsis(length, indent);}
public:
  int32_t write(const char* buffer, uint64_t length);             
  int32_t read(char*        buffer, uint64_t length, uint64_t offset=0); 
  void    terminate(); 
public:   
  unsigned update(XDS::Event* event) {return _data.update(event);}
public:
  int32_t error() const;
private:    
  void _synopsis(uint64_t length, int indent) const;
private:
  friend class Eraser;  
private:
  int32_t _remove();
private:
  Bucket         _bucket;
  Bucket         _data;
  SourceMetadata _metadata;
  int32_t        _error;
  DAQ::Location  _location;
};

}

#endif

