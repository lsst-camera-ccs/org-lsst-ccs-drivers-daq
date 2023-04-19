
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
 
#ifndef IMS_FOLDERS
#define IMS_FOLDERS

#include "dcs/Key.hh"
#include "ims/Catalog.hh"

namespace IMS {

class Folders {
public: 
  Folders()               = delete; 
  Folders(const Folders&) = delete;                 
public:                
  Folders(Catalog&);                 
public:
 virtual ~Folders();
public:
  void traverse(); 
public:
  virtual void process(const char* name) = 0;
private:
  int32_t   _error;
  DCS::Key* _keys;
  unsigned  _length;
  Catalog&  _catalog;      
  };

}

#endif


 
 