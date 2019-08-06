
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

#include "dsm/Exception.hh"
#include "xds/Store.hh"
#include "ims/Catalog.hh"

namespace IMS {

class Store : public XDS::Store {
public:
  Store()             = delete;
  Store(const Store&) = delete;
public:   
  Store(const char* partition, const char* interface) throw (DSM::Exception);
  Store(const char* partition)                        throw (DSM::Exception);
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

