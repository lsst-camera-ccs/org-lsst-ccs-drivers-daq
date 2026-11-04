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

#include "Add.hh"

static const char COMMAND[] = "add";

using namespace EMU;

/*
** ++
**
**
** --
*/

Editor::Add::Add(IMS::Store& store, const char* folder, PlayList* playlist) : 
 Command(COMMAND),
 _store(store),
 _folder(folder),
 _playlist(playlist)
 { 
 }

/*
** ++
**
**
** --
*/

#include "ims/Id.hh"
#include "ims/Image.hh"

#define IMAGE  argv[0]
#define FOLDER argv[1]

static const char NO_ARG[]       = "Need as a command argument at least one argument. The first is the name of the image to add while the second\n"
                                   "is the folder in which the image resides. If the second argument is omitted the default emulation folder is assumed\n";
static const char NO_PLAYLIST[]  = "A playlist was not passed as an argument when starting the utility\n"; 
static const char LOOKUP_ERROR[] = "Cannot locate '%s' contained in '%s'\n";
                           
void Editor::Add::process(int argc, const char** argv, int qualifier)   
 {
 
 PlayList* playlist = _playlist;
 
 if(!playlist) {printf(NO_PLAYLIST); return;}
 if(argc < 1)  {printf(NO_ARG);      return;}
 
 const char* folder = argc == 2 ? FOLDER : _folder;
 
 IMS::Id id = _store.catalog.lookup(IMAGE, folder);
 
 if(!id) {printf(LOOKUP_ERROR, IMAGE, folder); return;}
  
 playlist->insert(id);
   
 return; 
 }
    
/*
** ++
**
**
** --
*/

static const char HELP[] = "\n"
                           "  Add an entry into the current playlist. This command takes a single argument which\n"
                           "  specifies the storage location of the image to be added to the playlist. A storage\n"
                           "  location is specified as: 'image/folder'. Where: 'image' corresponds to the name of\n" 
                           "  the image to be added and 'folder' the name of its corresponding folder. For example:\n" 
                           "  <add my_image/in_my_folder> adds the image named 'my_image' in the folder named\n" 
                           "  'in_my_folder'. Note that if the folder is omitted from the specification a utility\n" 
                           "  local default is applied. For example, if the default folder was 'in_this_folder'\n" 
                           "  the command <add this_image> would have the same effect as the previous example.\n"
                           "  Note: Both image and folder can be no longer then 63 characters.\n"
                           "\n";

void Editor::Add::help(int argc, const char** argv, int qualifier)   
 {
 
 printf(HELP);
 
 return;
 }
    
