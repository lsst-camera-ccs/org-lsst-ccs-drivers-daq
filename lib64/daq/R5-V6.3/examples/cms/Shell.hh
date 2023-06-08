
/*
**  Package:
**	
**
**  Abstract:
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
 
#ifndef CMS_EDITOR_SHELL
#define CMS_EDITOR_SHELL

#include "ims/Shell.hh"
#include "cms/Camera.hh"

namespace CMS {namespace Editor {

class Shell : public IMS::Shell {
public:
  Shell(Camera&, const char* folder);
public:
 ~Shell() {}
public:
  void announce();
private:
  Camera& _camera;
  }; 
                 
}}

#endif
