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

#include "Stop.hh"

static const char COMMAND[] = "stop";

using namespace DSM;

/*
** ++
**
**
** --
*/

Stop::Stop(Interface& interface, Editor& editor) : 
 Command(COMMAND),
 _interface(interface),
 _editor(editor) 
 { 
 }

/*
** ++
**
**
** --
*/

static const char STOP[] = "Stop all locations of the partition (y or n)? ";

void Stop::process(int argc, const char** argv, int qualifier)   
 {
 
 if(!_editor.query(STOP)) return;
 
 _interface.shutdown();
 
 return;
 }

/*
** ++
**
**
** --
*/

static const char HELP[] = "\n"
                           "  This command stops all services, for all RCEs of the corresponding partition.\n"
                           "  It requires no arguments. To restart a partition's services see the <restart> command.\n"
                           "\n";

void Stop::help(int argc, const char** argv, int qualifier)   
 {
 
 printf(HELP);
 
 return;
 }
     
  
