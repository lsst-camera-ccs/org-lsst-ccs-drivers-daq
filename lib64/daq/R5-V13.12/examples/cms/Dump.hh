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
 
#ifndef CMS_EDITOR_DUMP
#define CMS_EDITOR_DUMP

#include "osa/Command.hh"
#include "cms/Camera.hh"

namespace CMS {namespace Editor {

class Dump : public OSA::Command { 
public:
  Dump(Camera&);
public:
 ~Dump() {}
public:
  void process(int argc, const char** argv, int qualifier);
  void help(   int argc, const char** argv, int qualifier);        
private:
  Camera& _camera;        
};

}}

#endif
