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
#include "ims/Store.hh"

namespace IMS {

class Bucket : public XDS::Bucket {
public:
  enum Type : uint8_t {IMAGE=0, METADATA=1, DATA=2};
public:
  Bucket() = delete;
  Bucket(const Id&,                                            Store&);  // Image metadata...
  Bucket(const Id&, Type, const DAQ::Location&,                Store&);  // Source read data or metadata...
  Bucket(const Id&, Type, const DAQ::Location&, uint64_t size, Store&);  // Source write data or metadata...
public:
  Bucket(const Bucket& clone) : XDS::Bucket(clone) {}
public:
 ~Bucket() {}
public:
  Bucket& operator=(const Bucket& clone) {*this = clone; return *this;}
};

}
 
#endif

