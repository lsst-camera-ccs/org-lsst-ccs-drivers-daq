
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
 
#ifndef OSA_FOLDERENTRIES
#define OSA_FOLDERENTRIES

#include "osa/Folder.hh"

namespace OSA {

class FolderEntries {
public:
  FolderEntries(const Folder&);
public:
 ~FolderEntries();
public:
  const char* next();   
public:
  operator bool() const {return _folder;}  
private:
  void* _folder;
};

}

#endif

