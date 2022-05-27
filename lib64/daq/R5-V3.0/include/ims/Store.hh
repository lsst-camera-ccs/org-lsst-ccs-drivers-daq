
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
 
#ifndef IMS_STORE
#define IMS_STORE

#include "ims/StoreBase.hh"
#include "ims/Catalog.hh"

namespace IMS {

class Store : public StoreBase {
public:
  Store()             = delete;
  Store(const Store&) = delete;
public:   
  Store(const char* partition);
  Store(uint8_t     partition);
public:
  Store(const char* partition, const char* interface);
  Store(uint8_t     partition, const char* interface);
public:
 ~Store() {}
public:
  Catalog catalog;
private:
  friend class Camera;
private:  
  Store(unsigned partition)                        throw(DSM::Exception);  
  Store(unsigned partition, const char* interface) throw(DSM::Exception);     
  };

}

#endif

