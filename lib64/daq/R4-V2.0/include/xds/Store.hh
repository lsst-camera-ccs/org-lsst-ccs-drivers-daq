
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

#include "net/ipv4/Address.hh"
#include "dvi/Version.hh"
#include "dsi/RandomSet.hh"
#include "dsi/LocationSet.hh"
#include "dsm/Client.hh"
#include "dsm/Exception.hh"
#include "xds/Stats.hh"

namespace XDS {

class Pending;     // Forward link...
class Transaction; // Forward link...
class Id;          // Forward link...

class Store {
public:  
  Store()             = delete; 
  Store(const Store&) = delete;
public:                         
  Store(const char* partition)                        throw (DSM::Exception);     
  Store(const char* partition, const char* interface) throw (DSM::Exception);                            
  Store(unsigned partition)                           throw (DSM::Exception);     
  Store(unsigned partition, const char* interface)    throw (DSM::Exception);    
public:
 ~Store();
public:
  const DSI::Set& servers() {return _client.servers();} 
public:
  bool probe(const DSI::Location&, DSI::Location& location, IPV4::Address&, DVI::Version&);
public:
  unsigned    partition() const {return _client.pid();}
  const char* interface() const {return _client.interface();} 
public:
  bool sample(const DSI::Location&, IPV4::Address&, Stats&, bool reset=false);
public:
  uint64_t capacity(DSI::LocationSet& missing);
  uint64_t remaining(DSI::LocationSet& missing);
private:
  friend class Bucket;
private:
  void _probe();
private:
  int32_t _get(const Id& id, void* buffer,       int length);
  int32_t _put(const Id& id, const void* buffer, int length);
private: 
  bool _write(Pending&);
  bool _read( Pending&);
  bool _erase(Pending&);
private:
  uint64_t _eob_get(const Id&);                // Get last page of bucket as specified by first argument, return in last argument
  int32_t  _eob_set(const Id&, uint64_t size); // Set last page of bucket as specified by the arguments
  uint64_t _eob_reset(const Id&);              // Remove EOB page of bucket as specified by first argument, return in last argument
private:
  bool _synch(Pending&);
  bool _synch_read(Pending&);
private:  
  void _copy(Transaction* expected, Transaction* actual, int length);
private:  
  const IPV4::Address& _destination(uint64_t key);
private: 
  DSM::Client    _client;
  IPV4::Address* _server; 
  DSI::RandomSet _random;
  int            _reordered;
  };

}

#endif

