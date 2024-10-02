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
 
#ifndef OSA_CATCHALL
#define OSA_CATCHALL

#include "osa/Command.hh"

#include <stdio.h>

namespace OSA {

class Catchall : public Command { 
public:
  Catchall();
public:
 ~Catchall() {}
public:
  void process(int argc, const char** argv, int qualifier);
  void help(   int argc, const char** argv, int qualifier);
};

}

#endif
