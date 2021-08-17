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
 
#ifndef XDS_EVENT
#define XDS_EVENT

#include "dvi/Endian.hh"
#include "xds/Id.hh" 

namespace XDS {

class __attribute__((__packed__)) Event {
public:
  Event() {}
  Event(const Id& bucket, uint64_t offset, uint32_t length, bool error, uint8_t marker=0);
  Event(const Event&);                                             
public:
 ~Event() {}
public:  
  Event& operator=(const Event&);
public:
  const Id& bucket() const {return _bucket;}  
  uint64_t  offset() const {return DVI::Endian::local(_offset);}
  uint32_t  length() const {return DVI::Endian::local(_length);}
  bool      error()  const {return _error;}
  uint8_t   marker() const {return _marker;}
public:
  bool accept(Id::Space space)                                 const {return _bucket.space() == space;} 
  bool accept(Id::Space space, uint8_t types)                  const {return accept(space) && (_bucket.type() & types);} 
  bool accept(Id::Space space, uint8_t types, uint64_t bucket) const {return accept(space) && (_bucket.type() & types) && bucket == _bucket.value();} 
public:
  void dump(int indent=0) const;
private:
  Id       _bucket; // Bucket ID
  uint32_t _length; // length (in bytes)
  uint64_t _offset; // Event offset (in bytes) within bucket
  uint8_t  _marker; // User defined
  uint8_t  _error;  // Error occurred in writing data
  uint8_t  _pad[6];
};

static_assert(!(sizeof(Event) & 7), "Class XDS::Event is not an even number of quadwords");

}
 
#endif

