/*
** ++
**  Package:
**	
**
**  Abstract:
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

#include <stdio.h>

#include "Quit.hh"

using namespace IMS;

/*
** ++
**
**
** --
*/
                                  
void Editor::Quit::process(int argc, const char** argv, int qualifier)
 {
   
 return;
 }

/*
** ++
**
**
** --
*/

static const char HELP[] = "\n"
                           "  The quit command causes the editor to exit. It requires no arguments\n"
                           "\n"; 

void Editor::Quit::help(int argc, const char** argv, int qualifier)   
 {
 
 printf(HELP);
 
 return;
 }
     
  
