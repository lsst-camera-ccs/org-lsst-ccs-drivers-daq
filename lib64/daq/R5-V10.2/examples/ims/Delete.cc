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

#include "Delete.hh"

static const char COMMAND[] = "delete";

using namespace IMS;

/*
** ++
**
**
** --
*/

Editor::Delete::Delete(Store& store, const char* folder) : 
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

#include "ims/Id.hh"
#include "ims/Image.hh"
#include "ims/Path.hh"

#define IMAGE  argv[0]
#define FOLDER argv[0]

static const char NOT_VALID[]    = "%s is not a valid image (first argument must be of the form: 'image-name/folder-name'). Is there a default folder?\n";
static const char LOOKUP_ERROR[] = "Cannot find '%s in the storage catalog\n";
static const char OPEN_ERROR[]   = "Cannot locate the metadata for '%s\n";

void Editor::Delete::process(int argc, const char** argv, int folder)   
 {
 
 if(!argc) return;

 if(!folder)
  {
   
  Path path(IMAGE, _default);
 
  if(!path) {printf(NOT_VALID, IMAGE); return;}
 
  Id id = _store.catalog.lookup(path.image(), path.folder());
 
  if(!id) {printf(LOOKUP_ERROR, IMAGE); return;}
 
  Image image(id, _store);
 
  if(!image) {printf(OPEN_ERROR, IMAGE); return;}
     
 image.remove();
 
  }
 else
 _delete(FOLDER); 
 
 return;
 }  

/*
** ++
**
**
** --
*/

#include "dcs/Exception.hh"
#include "ims/Folder.hh"
#include "Deleter.hh"

static const char ERROR[] = "Cannot locate storage folder '%s' (error = %s)\n";

void Editor::Delete::_delete(const char* name)
 {
 
 Folder folder(name, _store.catalog);

 int32_t error = folder.error();
 
 if(error) printf(ERROR, name, DCS::Exception::decode(error));
 
 Deleter deleter(_store);
  
 folder.traverse(deleter);
  
 deleter.summary();
 
 return;
 }  
     
/*
** ++
**
**
** --
*/

static const char HELP[] = "\n"
                           "  Remove one or more images from the store. This command takes a single argument and\n" 
                           "  optionally a single qualifier which modifies the argument's meaning. That qualifier\n" 
                           "  is '/all'. If the qualifier is present the argument is the name of a storage folder\n"  
                           "  and all its images are deleted. For example: <delete/all my_folder> would remove all\n"
                           "  images in the folder named 'my_folder.'\n"
                           "  If the qualifier is not present the argument specifies the storage location of the image\n"
                           "  to be deleted. A storage location is specified as: 'image/folder'. Where: 'image'\n" 
                           "  corresponds to the name of the image to be deleted and 'folder' the name of its\n" 
                           "  corresponding folder. For example: <delete my_image/in_my_folder> removes the image named\n"
                           "  'my_image' from the folder named 'in_my_folder'. Note that if the folder is omitted\n" 
                           "  from the specification a utility local default is applied. For example, if the default\n"
                           "  folder was 'in_this_folder' the command <delete this_image> would have the same effect\n" 
                           "  as the previous example. Note: Both image and folder can be no longer then 63 characters.\n"
                           "\n";

void Editor::Delete::help(int argc, const char** argv, int qualifier)   
 {
 
 printf(HELP);
 
 return;
 }
    
