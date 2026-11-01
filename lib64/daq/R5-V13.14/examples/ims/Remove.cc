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

#include "Remove.hh"

static const char COMMAND[] = "remove";

using namespace IMS;

/*
** ++
**
**
** --
*/

Editor::Remove::Remove(Catalog& catalog) : 
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
#include "ims/Folder.hh"

#define FOLDER argv[0] 

static const char NOARG[]     = "Need as a command arguments either the name of the image as well as its containing folder (the image name is first), or just a folder\n";
static const char ERROR[]     = "Cannot locate  folder '%s' (error = %s)\n";
static const char NOT_EMPTY[] = "Cannot delete a folder which is not empty (see <delete>)\n";

void Editor::Remove::process(int argc, const char** argv, int constant)   
 {
  
 if(!argc) {printf(NOARG); return;}
 
 Folder folder(FOLDER, _catalog);

 int32_t error = folder.error();
 
 if(error) printf(ERROR, FOLDER, DCS::Exception::decode(error));
 
 if(folder.length()) printf(NOT_EMPTY);
 
 _catalog.remove(FOLDER);
  
 return; 
 }
     
/*
** ++
**
**
** --
*/

static const char HELP[] = "\n"
                           "  Remove a folder from the store. Folders can only be removed if they are empty.\n"
                           "  See the <delete> command to empty a folder. The command takes a single argument\n"
                           "  which names the folder to be removed\n"
                           "\n";
                        
void Editor::Remove::help(int argc, const char** argv, int qualifier)   
 {
 
 printf(HELP);
 
 return;
 }
    
