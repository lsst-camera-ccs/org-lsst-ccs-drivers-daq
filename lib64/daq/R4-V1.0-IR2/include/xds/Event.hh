
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
 
#ifndef XDS_EVENT
#define XDS_EVENT

#include <stdint.h>

#include "net/Endian.hh"
#include "xds/Id.hh"

namespace XDS {

class Event {
public:
  Event() = delete;
public:
  Event(const Id& bucket,  uint64_t offset, uint64_t length) : _bucket(bucket),        _offset(Net::Endian::wire(offset)), _length(Net::Endian::wire(length)) {}
  Event(const Id& bucket,                   uint64_t length) : _bucket(bucket),        _offset(uint64_t(-1)),              _length(Net::Endian::wire(length)) {} // EOB...
  Event(const Event& clone)                                  : _bucket(clone._bucket), _offset(clone._offset),             _length(clone._length)             {}
public:
 ~Event() {}
public:  
  Event& operator=(const Event& clone) {_bucket = clone._bucket; _offset = clone._offset; _length = clone._length; return *this;}
public:
  uint64_t  offset() const {return Net::Endian::local(_offset);}    
  uint64_t  length() const {return Net::Endian::local(_length);}    
  const Id& bucket() const {return _bucket;}    
public:
  void lengthIs(uint64_t value) {_length = Net::Endian::wire(value);}
  void offsetIs(uint64_t value) {_offset = Net::Endian::wire(value);}
public:
  void dump(int indent=0) const;   
private:
  Id       _bucket;
  uint32_t _pad;
  uint64_t _offset;
  uint64_t _length;
  };
  
}

#endif

