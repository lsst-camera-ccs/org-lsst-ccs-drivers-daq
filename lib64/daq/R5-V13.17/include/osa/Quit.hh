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
 
#ifndef OSA_QUIT
#define OSA_QUIT

#include "osa/Command.hh"

namespace OSA {

class Quit : public Command { 
public:
  Quit();
public:
 ~Quit() {}
};

}

#endif
