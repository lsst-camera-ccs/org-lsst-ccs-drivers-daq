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

#include "Insert.hh"

static const char COMMAND[] = "insert";

using namespace IMS;

/*
** ++
**
**
** --
*/

Editor::Insert::Insert(Catalog& catalog) :
 Command(COMMAND), 
 _catalog(catalog) 
 { 
 }

/*
** ++
**
**
** --
*/

#include "dcs/Exception.hh"

#define FOLDER argv[0]

static const char NOARG[]   = "One and only one argument is allowed. That argument is the name of the folder to create.\n";
static const char ERROR[]   = "Can't create '%s' (%s)\n";

static const char ID[] = "Found ID:%s assigned to %s\n";
                              
void Editor::Insert::process(int argc, const char** argv, int qualifier)   
 {

 if(argc != 1) {printf(NOARG); return;}
  
 int32_t error = _catalog.insert(FOLDER);
   
 if(error)  printf(ERROR, FOLDER, DCS::Exception::decode(error));
    
 return; 
 }
 
/*
** ++
**
**
** --
*/

static const char HELP[] = "\n"
                           "  Create a new folder. The first (and only) argument is a string\n"
                           "  corresponding to the name to be assigned to the created folder.\n"
                           "\n";
                              
void Editor::Insert::help(int argc, const char** argv, int qualifier)   
 {
 
 printf(HELP);
 
 return;
 }
 
