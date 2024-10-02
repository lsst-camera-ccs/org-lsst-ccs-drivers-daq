
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

#include "dcs/Catalog.hh"
#include "dcs/Id.hh"
#include "dcs/Key.hh"
#include "dcs/Entry.hh"
#include "ims/Id.hh"

namespace IMS {

class Catalog {
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
  Catalog(unsigned partition);
  Catalog(unsigned partition, const char* interface);
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

