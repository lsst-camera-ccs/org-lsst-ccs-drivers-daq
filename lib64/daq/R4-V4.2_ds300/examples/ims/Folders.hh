
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
 
#ifndef IMS_EDITOR_FOLDERS
#define IMS_EDITOR_FOLDERS

#include "ims/Catalog.hh"
#include "ims/Folders.hh"

namespace IMS {namespace Editor {

class Folders : public IMS::Folders {
public:
  Folders()               = delete; 
  Folders(const Folders&) = delete;  
public:
  Folders(Catalog&);
public:
 ~Folders() {} 
public:
  void process(const char* name);
public:
  void summary() const;                               
private:
  unsigned _total;
};
 
}}

#endif

