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

#include "Version.hh"

static const char COMMAND[] = "version";

using namespace IMS;

/*
** ++
**
**
** --
*/

Editor::Version::Version(Store& store) :
 Command(COMMAND), 
 _store(store)
 { 
 }

/*
** ++
**
**
** --
*/

#include "dvi/Version.hh"

static const char CLEAN[]   = "Version: %s [08%X] (clean), %-24s\n";
static const char DIRTY[]   = "Version: %s [08%X] (dirty), %-24s\n";                               
                           
void Editor::Version::process(int argc, const char** argv, int constant)   
 {

 DVI::Version version;
     
 printf(version.dirty() ? DIRTY : CLEAN, version.tag(), version.hash(), version.buildTime.decode());

 return; 
 }
  
/*
** ++
**
**
** --
*/

static const char HELP[] = "\n"
                          "  Prints to standard out the release information of the editor itself.\n"
                          "\n";
                              
void Editor::Version::help(int argc, const char** argv, int qualifier)   
 {
 
 printf(HELP);
 
 return;
 }
 
