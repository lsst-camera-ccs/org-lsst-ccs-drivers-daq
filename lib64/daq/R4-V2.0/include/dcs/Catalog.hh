
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
 
#ifndef DCS_CATALOG
#define DCS_CATALOG

#include "net/ipv4/Address.hh"
#include "dvi/Version.hh"
#include "dsm/Client.hh"
#include "dsm/Exception.hh"

#include "dcs/Id.hh"
#include "dcs/Key.hh"
#include "dcs/Entry.hh"

namespace DCS {

class Catalog {
public:
  static unsigned max_names();
public: 
  Catalog()               = delete; 
  Catalog(const Catalog&) = delete;  
public:                         
  Catalog(const char* partition)                        throw(DSM::Exception);  
  Catalog(const char* partition, const char* interface) throw(DSM::Exception);  
  Catalog(unsigned    partition, const char* interface) throw(DSM::Exception);  
  Catalog(unsigned    partition)                        throw(DSM::Exception);  
public:
 ~Catalog(); 
public:
  const DSI::Set& servers() {return _client.servers();} 
public:  
  bool probe(const DSI::Location&, DSI::Location&, IPV4::Address&, DVI::Version&); 
public:
  int32_t insert(const char* folder);
  int32_t lookup(const Key&  folder, Entry&);
  int32_t remove(const Key&  folder);
public:
  int32_t assign(const char* name, const Key&  folder, const Id&);   
  int32_t lookup(const Key&  name, const Key& folder, Entry&);
  int32_t move(  const Key&  name, const Key& folder, const char* destination); 
  int32_t remove(const Key&  name, const Key& folder);
  int32_t remove(const Id&);
public:
  int marshall(Key keys[]);
  int marshall(const char* folder, Key keys[], int32_t& error);
  int marshall(const char* folder, Id  ids[],  int32_t& error);
private: 
  DSM::Client _client;
  };

}

#endif

