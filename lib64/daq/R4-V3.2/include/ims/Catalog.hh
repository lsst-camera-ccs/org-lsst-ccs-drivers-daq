
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
 
#ifndef IMS_CATALOG
#define IMS_CATALOG

#include "dsm/Exception.hh"
#include "dcs/Catalog.hh"
#include "dcs/Id.hh"
#include "dcs/Key.hh"
#include "dcs/Entry.hh"
#include "ims/Id.hh"

namespace IMS {

class Catalog {
public:
  Catalog()               = delete;
  Catalog(const Catalog&) = delete; 
public:  
  Catalog(const char* partition, const char* interface) throw (DSM::Exception);
  Catalog(const char* partition)                        throw (DSM::Exception);
public:
 ~Catalog() {}
public:
  bool probe( const char* name, const char* folder);
  Id   lookup(const char* name, const char* folder);
public:  
  int32_t insert(const char* folder);
  int32_t remove(const char* folder);
private:
  friend class Image;
  friend class Folder;
  friend class Folders;
  friend class Store;
private:  
  Catalog(unsigned partition)                        throw(DSM::Exception);  
  Catalog(unsigned partition, const char* interface) throw(DSM::Exception);  
private:
  int32_t _assign(const char* name, const char* folder, const Id&);
  int32_t _remove(const Id&);
  int32_t _lookup(const DCS::Key&, DCS::Entry&); 
private:
  int _marshall(const char* folder, DCS::Id ids[], int32_t& error);  
  int _marshall(DCS::Key keys[]);  
private:
  DCS::Catalog _catalog;
  };

}

#endif

