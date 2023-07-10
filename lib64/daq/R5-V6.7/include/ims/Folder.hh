
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
 
#ifndef IMS_FOLDER
#define IMS_FOLDER

#include "dcs/Id.hh"
#include "ims/Id.hh"
#include "ims/Catalog.hh"
#include "ims/Processor.hh"

namespace IMS {

class Folder {
public: 
  Folder()              = delete; 
  Folder(const Folder&) = delete;                 
public:                
  Folder(const char* name, Catalog&);                 
public:
 ~Folder();
public:
  operator bool() const;
public:
  unsigned length() const;
public:
  void traverse(Processor&); 
public:  
  int32_t error() const;
private:
  int32_t  _error;
  DCS::Id* _ids;
  unsigned _length;      
  };

}

#endif


 
 