/*
** ++
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
**	000 - January 09, 2007
**
**  Revision History:
**	None.
**
** --
*/
 
#ifndef CMS_EDITOR_TRIGGER
#define CMS_EDITOR_TRIGGER

#include "osa/Command.hh"
#include "cms/Camera.hh"

namespace CMS {namespace Editor {

class Trigger : public OSA::Command { 
public:
  Trigger(Camera&);
public:
 ~Trigger() {}
public:
  void process(int argc, const char** argv, int qualifier);
  void help(   int argc, const char** argv, int qualifier);        
private:
  Camera& _camera;        
};

}}

#endif
