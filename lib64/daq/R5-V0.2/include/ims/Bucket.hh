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
 
#ifndef IMS_BUCKET
#define IMS_BUCKET

#include "daq/Location.hh"
#include "xds/Bucket.hh"
#include "ims/Id.hh"
#include "ims/StoreBase.hh"

namespace IMS {

class Bucket : public XDS::Bucket {
public:
  enum {MAX=2048}; // Maximum bucket (in units of pages)... 
public:
  enum Type : uint8_t {IMAGE = 1 << 0, METADATA = 1 << 1, DATA = 1 << 2};
public:
  Bucket(const Id&,                                            StoreBase&);  // Image metadata...
  Bucket(const Id&, Type, const DAQ::Location&,                StoreBase&);  // Source read data or metadata...
  Bucket(const Id&, Type, const DAQ::Location&, uint64_t size, StoreBase&);  // Source write data or metadata...
public:
  Bucket(const Bucket& clone) : XDS::Bucket(clone) {}
public:
  Bucket() = delete;
public:
 ~Bucket() {}
public:
  Bucket& operator=(const Bucket& clone) {*this = clone; return *this;}
public:
  int32_t write(const char* buffer,   uint64_t length);
  int32_t publish(const char* buffer, uint64_t length, uint8_t marker=0);
public:
  uint64_t size(  uint32_t pages=MAX);
  uint64_t remove(uint32_t pages=MAX);   
private:
  uint64_t _offset; 
};

}
 
#endif

