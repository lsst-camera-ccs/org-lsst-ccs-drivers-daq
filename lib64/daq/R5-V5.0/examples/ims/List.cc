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

#include "List.hh"

static const char COMMAND[] = "list";

using namespace IMS;

/*
** ++
**
**
** --
*/

Editor::List::List(Store& store) :
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

#include "ims/Folder.hh"

#include "Printer.hh"
#include "Folders.hh"

#define FOLDER argv[0]

void Editor::List::process(int argc, const char** argv, int constant)   
 {

 if(argc)
  {
  
  Folder folder(FOLDER, _store.catalog);
 
  Printer printer(_store);
  
  folder.traverse(printer);
  
  printer.summary();
  }
 else
  {
  
  Folders folders(_store.catalog);
 
  folders.traverse();
  
  folders.summary();
  }

 return; 
 }
  
/*
** ++
**
**
** --
*/

static const char HELP[] = "\n"
                           "  Prints to standard output either a list of folders or a list of all images within\n"
                           "  a folder. If no argument is supplied the output is a list of all folders in the store.\n"
                           "  If a single argument is supplied, it corresponds to the name of a folder. In such\n"
                           "  a case all images within that folder are output\n"
                           "\n";
                              
void Editor::List::help(int argc, const char** argv, int qualifier)   
 {
 
 printf(HELP);
 
 return;
 }
 
