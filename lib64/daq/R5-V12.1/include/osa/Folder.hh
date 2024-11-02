
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
 
#ifndef OSA_FOLDER
#define OSA_FOLDER

#include "osa/Path.hh"

#include <fcntl.h> // Needed for access flags...

namespace OSA {

class Folder : public Path {
public:
 enum : mode_t {ACCESS1=((S_IRUSR | S_IWUSR | S_IXUSR) | (S_IRGRP | S_IXGRP) | (S_IROTH | S_IXOTH))};
 enum : mode_t {ACCESS2=((S_IRUSR | S_IWUSR)           | (S_IRGRP | S_IWGRP) | (S_IROTH | S_IWOTH))};
public:
  Folder(const char* path,                      mode_t = ACCESS1);
  Folder(const char* parent, const char* child, mode_t = ACCESS1);
public:
 ~Folder() {}
};

}

#endif

