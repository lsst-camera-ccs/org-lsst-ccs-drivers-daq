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
 
#ifndef IMS_EDITOR_QUIT
#define IMS_EDITOR_QUIT

#include "osa/Quit.hh"

namespace IMS {namespace Editor {

class Quit : public OSA::Quit { 
public:
  Quit() : OSA::Quit() {}
public:
 ~Quit() {}
public:
  void process(int argc, const char** argv, int qualifier);
  void help(   int argc, const char** argv, int qualifier);
};

}}

#endif
