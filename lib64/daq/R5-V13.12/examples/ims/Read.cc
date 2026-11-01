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

#include "Read.hh"

static const char COMMAND[] = "read";

static float _rate(uint64_t size, long interval);

using namespace IMS;

/*
** ++
**
**
** --
*/

Editor::Read::Read(Store& store, const char* folder) : 
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

#include "osa/Timer.hh"
#include "ims/Id.hh"
#include "ims/Path.hh"
#include "ims/Image.hh"
#include "Reader.hh"

#define IMAGE argv[0]

static const char NOT_VALID[]    = "%s is not a valid image (first argument must be of the form: 'image-name/folder-name'). Is there a default folder? \n";
static const char LOOKUP_ERROR[] = "Cannot find '%s in the storage catalog\n";
static const char OPEN_ERROR[]   = "Cannot locate the metadata for '%s\n";
static const char READ[]         = "Read %llu byte(s) of data in %ldus (%.0f Mbytes/second)\n"; 
                           
void Editor::Read::process(int argc, const char** argv, int qualifier)   
 {
 
 if(!argc) return;
 
 Path path(IMAGE, _default);
 
 if(!path) {printf(NOT_VALID, IMAGE); return;}
 
 Id bucket = _store.catalog.lookup(path.image(), path.folder());
 
 if(!bucket) {printf(LOOKUP_ERROR, IMAGE); return;}
 
 Image image(bucket, _store);
 
 if(!image) {printf(OPEN_ERROR, IMAGE); return;}

 Image::title(); image.synopsis();
 
 Reader reader(image);
   
 OSA::Timer timer; timer.start(); 
   
 reader.run();
 
 long duration = timer.stop();

 uint64_t processed = reader.processed();
 
 printf(READ, (long long unsigned)processed, duration, _rate(processed, duration));
   
 return; 
 }

/*
** ++
**
**
** --
*/
 
#define SECONDS (float)(1000 * 1000)
#define MBYTES  (float)(1024 * 1024) 
      
float _rate(uint64_t size, long interval)
 {

 float mbytes = (float)size / MBYTES;
 float us     = (float)interval; 
 float rate   = (mbytes/us) * SECONDS;
 
 return rate; 
 }
    
/*
** ++
**
**
** --
*/

static const char HELP[] = "\n"
                           "  Read the contents of a specified image. This command takes a single argument which\n"
                           "  specifies the storage location of the image to be read. A storage location is\n"
                           "  specified as: 'image/folder'. Where: 'image' corresponds to the name of the image\n" 
                           "  to be read and 'folder' the name of its corresponding folder. For example:\n" 
                           "  <read my_image/in_my_folder> reads the image named 'my_image' in the folder named\n" 
                           "  'in_my_folder'. Note that if the folder is omitted from the specification a utility\n" 
                           "  local default is applied. For example, if the default folder was 'in_this_folder'\n" 
                           "  the command <read this_image> would have the same effect as the previous example.\n"
                           "  Note: Both image and folder can be no longer then 63 characters.\n"
                           "\n";

void Editor::Read::help(int argc, const char** argv, int qualifier)   
 {
 
 printf(HELP);
 
 return;
 }
    
