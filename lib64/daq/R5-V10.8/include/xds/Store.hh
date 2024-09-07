
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
 
#ifndef XDS_STORE
#define XDS_STORE

#include "dsm/Client.hh"

#include "net/ipv4/Address.hh"
#include "dsi/Header.hh"
#include "dsi/Set.hh"
#include "dsi/RandomSet.hh"
#include "xds/Page.hh"
#include "xds/Stats.hh"

namespace XDS {

class Store : public DSM::Client {
public:                         
  Store(const char* partition);
  Store(uint8_t     partition);
public:                         
  Store(const char* partition, const char* interface);
  Store(uint8_t     partition, const char* interface);
public:  
  Store()             = delete; 
  Store(const Store&) = delete;
public:
 ~Store();
public:
  const DSI::LocationSet& servers() {return (const DSI::LocationSet&)_servers;} 
public:
  unsigned pages()         const {return _pages;}
  unsigned maxpages()      const {return _maxpages;}
  unsigned numof_servers() const {return _numof_servers;}
  unsigned capacity()      const {return _pages*_numof_servers;}
  unsigned remaining();
  bool     consistent()    const {return _consistent;}
public:
  void config(DSI::LocationSet& servers, DSI::LocationSet locs[], uint32_t pages[], uint32_t maxpages[]);
public:
  bool reset(uint32_t pages=0);
  bool wakeup();
public:
  operator bool() const {return _consistent;}
public:
  bool sample(Stats stats[], bool reset=false);
public:
  const IPV4::Address& lookup(const Page& page) const {return _lookup(page.id());}
  uint8_t              resolve  (uint64_t page) const {return _map.lookup(page);}
public:
  virtual bool reinit();
private:
  friend class Bucket;
  friend class Writer;
  friend class Reader;
private:
  const IPV4::Address& _lookup( uint64_t page) const {uint8_t server = _map.lookup(page); return _server[server];}
private:
  bool     _probe(     const DSI::Header& request, DSI::Set& missing);
  void     _wait(      const DSI::Header& request, DSI::Set& missing);
  unsigned _remaining( const DSI::Header& request, DSI::Set& missing);
  void     _sample(    const DSI::Header& request, DSI::Set& missing, Stats vector[]);
  void     _config(    const DSI::Header& request, DSI::Set& missing, DSI::LocationSet locs[], uint32_t pages[], uint32_t maxpages[]);
  bool     _probe();
private:
  DSI::RandomSet _map;
  IPV4::Address  _server[DSI::Set::SIZE];
private:
  unsigned _pages;
  unsigned _maxpages;
  unsigned _numof_servers;
  bool     _consistent;
  };

}

#endif

