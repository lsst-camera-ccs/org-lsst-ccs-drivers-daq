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

#include "ims/Exception.hh"
#include "Inspector.hh"

#include "Inspect.hh"

static const char COMMAND[] = "inspect";

using namespace IMS;

/*
** ++
**
**
** --
*/

Editor::Inspect::Inspect(Store& store, const char* folder) :
 Command(COMMAND), 
 _store(store),
 _default(folder)
 { 
 
 }

/*
** ++
**
**
** --
*/

#include "ims/Image.hh"
#include "ims/Path.hh"

#define IMAGE argv[0]

static const char CR[]           = "\n";
static const char NOT_VALID[]    = "%s is not a valid image (first argument must be of the form: 'image-name/folder-name'). Is there a default folder?\n";
static const char LOOKUP_ERROR[] = "Cannot find '%s in the storage catalog\n";
static const char OPEN_ERROR[]   = "Cannot locate the metadata for '%s\n";

#define INDENT 2
                         
void Editor::Inspect::process(int argc, const char** argv, int constant)   
 {

 if(!argc) return;
 
 Path path(IMAGE, _default);
 
 if(!path) {printf(NOT_VALID, IMAGE); return;}
 
 Id bucket = _store.catalog.lookup(path.image(), path.folder());
 
 if(!bucket) {printf(LOOKUP_ERROR, IMAGE); return;}
 
 Image image(bucket, _store);
 
 if(!image) {printf(OPEN_ERROR, IMAGE); return;}
  
 printf(CR);
   
 Image::title(); image.synopsis();
 
 printf(CR);
 
 Editor::Inspector inspector(image);
 
 inspector.run();
 
 inspector.summary();
   
 return; 
 }

/*
** ++
**
**
** --
*/

static const char HELP[] = "\n"
                           "  Prints to standard out a synopsis of a specified image and its corresponding elements.\n"
                           "  This command takes a single argument specifying the storage location of the image to be\n" 
                           "  inspected. A storage location is specified as: 'image/folder'. Where: 'image' corresponds\n" 
                           "  to the name of the image to be inspected and 'folder' the name of its corresponding folder.\n"
                           "  For example: <inspect my_image/in_my_folder> inspects the image named 'my_image' in the folder\n" 
                           "  named 'in_my_folder'. Note that if the folder is omitted from the specification a utility\n" 
                           "  local default is applied. For example, if the default folder was 'in_this_folder' the command\n"
                           "  <inspect this_image> would have the same effect as the previous example. Note: Both image and\n" 
                           "  folder can be no longer then 63 characters.\n"
                           "\n";

void Editor::Inspect::help(int argc, const char** argv, int qualifier)   
 {
 
 printf(HELP);
 
 return;
 }
 
