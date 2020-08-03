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

#include "Restart.hh"

static const char COMMAND[] = "restart";

using namespace DSM;

/*
** ++
**
**
** --
*/

Restart::Restart(Interface& interface, Editor& editor) : 
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

static const char RESTART[] = "Restart all locations of the partition (y or n)? ";

void Restart::process(int argc, const char** argv, int qualifier)   
 {
 
 if(!_editor.query(RESTART)) return;
 
 _interface.restart();
 
 return;
 }

/*
** ++
**
**
** --
*/

static const char HELP[] = "\n"
                           "  This command restarts all services, for all RCEs of the corresponding partition.\n"
                           "  It requires no arguments. To stop a partition's services see the <stop> command.\n"
                           "\n";
                           
void Restart::help(int argc, const char** argv, int qualifier)   
 {
 
 printf(HELP);
 
 return;
 }
     
  
