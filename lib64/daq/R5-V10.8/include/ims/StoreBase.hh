
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

#ifndef IMS_STOREBASE
#define IMS_STOREBASE

#include "xds/Store.hh"

namespace IMS {

class StoreBase : public XDS::Store {
public:
  StoreBase()                 = delete;
  StoreBase(const StoreBase&) = delete;
public:
  StoreBase(const char* partition);
  StoreBase(uint8_t     partition);
public:
  StoreBase(const char* partition, const char* interface);
  StoreBase(uint8_t     partition, const char* interface);
public:
 ~StoreBase() {}
private:
  StoreBase(unsigned partition);
  StoreBase(unsigned partition, const char* interface);
};

}

#endif

