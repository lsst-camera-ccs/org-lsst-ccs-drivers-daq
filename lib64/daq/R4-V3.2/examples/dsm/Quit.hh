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
 
#ifndef DSM_QUIT
#define DSM_QUIT

#include "osa/Quit.hh"
#include "dsm/Editor.hh"

namespace DSM {

class Quit : public OSA::Quit { 
public:
  Quit(Editor&);
public:
 ~Quit() {}
public:
  void process(int argc, const char** argv, int qualifier);
  void help(   int argc, const char** argv, int qualifier);
private:
  Editor& _editor;     
};

}

#endif
