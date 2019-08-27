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
 
#ifndef XDS_SERVICESTORE
#define XDS_SERVICESTORE

#include "xds/Exception.hh"
#include "xds/Page.hh"
#include "xds/Stats.hh"
#include "Inuse.hh"
#include "Freelist.hh"
#include "Buffer.hh"

namespace XDS {

class ServiceStore {
public:
  ServiceStore() = delete;
  ServiceStore(unsigned pages, const char* directory);
public:
 ~ServiceStore();
public:
  const void* lookup(const Page*);
public:
  enum Exception::Error insert(const Page*);
  enum Exception::Error remove(const Page*);
public:                     
  enum Exception::Error terminate(const Page*);
public:  
  uint64_t get(  const Page*);
  uint64_t reset(const Page*);
public:
  uint32_t remaining() const {return _freelist.remaining();}
public:
  Stats stats;
private:
  Inuse    _inuse;
  Freelist _freelist;
  Buffer   _content;
};

}
 
#endif

