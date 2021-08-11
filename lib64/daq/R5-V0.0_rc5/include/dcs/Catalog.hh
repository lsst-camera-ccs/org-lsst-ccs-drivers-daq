
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
#include "dsm/Client.hh"
#include "dcs/Id.hh"
#include "dcs/Key.hh"
#include "dcs/Entry.hh"

namespace DCS {

class Catalog : public DSM::Client {
public:
  static unsigned max_names();
public:                         
  Catalog(const char* partition);  
  Catalog(uint8_t     partition);  
public:                         
  Catalog(const char* partition, const char* interface);  
  Catalog(uint8_t     partition, const char* interface);  
public: 
  Catalog()               = delete; 
  Catalog(const Catalog&) = delete;  
public:
 ~Catalog(); 
public:
  int32_t insert(const char* folder);
  int32_t lookup(const Key&  folder, Entry&);
  int32_t remove(const Key&  folder);
public:
  int32_t assign(const char* name, const Key& folder, const Id&);   
  int32_t lookup(const Key&  name, const Key& folder, Entry&);
  int32_t move(  const Key&  name, const Key& folder, const char* destination); 
  int32_t remove(const Key&  name, const Key& folder);
  int32_t remove(const Id&);
public:
  int32_t marshall(Key keys[]);
  int32_t marshall(const char* folder, Key keys[]);
  int32_t marshall(const char* folder, Id  ids[]);
private:
  int32_t _wait_on(const DSI::Header&);
  int32_t _wait_on(const DSI::Header&, int32_t error);
  };

}

#endif

