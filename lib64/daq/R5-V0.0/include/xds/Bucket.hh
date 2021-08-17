
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

#include "net/ipv4/Address.hh"
#include "dsi/Set.hh"
#include "xds/Id.hh"
#include "xds/Counters.hh"
#include "xds/Store.hh"
#include "xds/Exception.hh"
#include "xds/Page.hh"
#include "xds/StreamAddress.hh"

namespace XDS {

class Bucket {
public: 
  Bucket(const Id&, Store&);                                                                                                             
  Bucket(const Bucket&); 
public:
  Bucket() = delete;
public:
 ~Bucket() {}
public:  
  Bucket& operator=(const Bucket&);
public:
  Exception::Error write(  const char* content, uint32_t size, uint64_t offset=0);
  Exception::Error read(   char*       content, uint32_t size, uint64_t offset=0); 
  Exception::Error publish(const char* content, uint32_t size, uint64_t offset, uint8_t marker=0);
public:
  unsigned allocation(uint32_t size, DSI::Set& missing, unsigned histogram[], uint32_t offset = 0);  
public:
  uint64_t remove(uint32_t pages, DSI::Set& missing, uint32_t offset = 0);
  uint64_t lookup(uint32_t pages, DSI::Set& missing, uint32_t offset = 0);  
public:
  const Counters& counters() const {return _counters;}
private: 
  uint64_t _total(     const DSI::Header& request, DSI::Set& missing, Counter&);
  unsigned _allocation(const DSI::Header& request, DSI::Set& missing, Counter&, unsigned histogram[]);
private:
  Id            _bucket;
  Store&        _store;
  Counters      _counters;
  StreamAddress _stream;
  };

}

#endif

