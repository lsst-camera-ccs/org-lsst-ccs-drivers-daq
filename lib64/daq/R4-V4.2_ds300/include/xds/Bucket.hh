
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
 
#ifndef XDS_BUCKET
#define XDS_BUCKET

#include "xds/Id.hh"
#include "xds/Event.hh"
#include "xds/Store.hh"

namespace XDS {

class Bucket {
public:
 enum : unsigned {DISCARD = 0, TRANSFER = 1 << 0, DONE = 1 << 1};
public: 
#if defined(__rtems__)
  enum {DEFAULT_TRANSFER = (64*1024), ONE_TRANSFER = (1024*32)};
#else
  enum {DEFAULT_TRANSFER = (1024*1024), ONE_TRANSFER = (1024*32)};
#endif
public:   
  Bucket(const Bucket&); 
  Bucket(const Id&,                Store&, uint32_t max_transfer = DEFAULT_TRANSFER);                                                                                                           
  Bucket(const Id&, uint64_t size, Store&, uint32_t max_transfer = DEFAULT_TRANSFER);                                                       
public:
  Bucket() = delete;
public:
 ~Bucket() {delete[] _buffer;}
public:  
  Bucket& operator=(const Bucket&);
public:  
  uint32_t max_transfer() const {return _max_transfer;}
  uint64_t size()         const {return _size;} 
public:
  int32_t  write(const char* content, uint32_t size);
  int32_t  read( char*       content, uint32_t size, uint64_t offset=0);
  int32_t  erase(uint64_t size);
  int32_t  get(void* buffer,       int length) {return _store._get(_bucket, buffer, length);}
  int32_t  put(const void* buffer, int length) {return _store._put(_bucket, buffer, length);}
  int32_t  terminate()                         {return _store._eob_set(_bucket, _offset);}
  uint64_t reset()                             {return _store._eob_reset(_bucket);}
public:  
  unsigned update(Event*);
public:
  void dump(int indent=0) const {return _bucket.dump();}   
private:
  Id       _bucket;
  uint64_t _size;
  uint64_t _offset;
  Store&   _store;
  char*    _buffer;
  uint32_t _max_transfer;
  };

}

#endif

