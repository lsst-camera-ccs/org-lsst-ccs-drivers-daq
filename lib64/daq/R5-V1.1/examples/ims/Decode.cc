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

#include "Decode.hh"

static const char COMMAND[] = "decode";

static float _rate(uint64_t size, long interval);

using namespace IMS;

/*
** ++
**
**
** --
*/

Editor::Decode::Decode(Store& store, const char* folder) : 
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
#include "ims/Image.hh"
#include "ims/Path.hh"
#include "Decoder.hh"

#define IMAGE argv[0]

static const char NOT_VALID[]    = "%s is not a valid image (first argument must be of the form: 'image-name/folder-name'). Is there a default folder?\n";
static const char LOOKUP_ERROR[] = "Cannot find '%s in the storage catalog\n";
static const char OPEN_ERROR[]   = "Cannot locate the metadata for '%s\n";
static const char DECODE[]       = "Decoded %llu byte(s) of data in %ldus (%.0f Mbytes/second)\n"; 
                           
void Editor::Decode::process(int argc, const char** argv, int qualifier)   
 {
 
 if(!argc) return;
 
 Path path(IMAGE, _default);
 
 if(!path) {printf(NOT_VALID, IMAGE); return;}
 
 Id bucket = _store.catalog.lookup(path.image(), path.folder());
 
 if(!bucket) {printf(LOOKUP_ERROR, IMAGE); return;}
 
 Image image(bucket, _store);
 
 if(!image) {printf(OPEN_ERROR, IMAGE); return;}

 Image::title(); image.synopsis();
 
 Decoder decoder(image);
   
 OSA::Timer timer; timer.start(); 
   
 decoder.run();
 
 long duration = timer.stop();

 uint64_t size = decoder.size();
 
 printf(DECODE, (long long unsigned)size, _rate(size, duration));
   
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
                           "  Read and decode the contents of a specified image. This command takes a single argument\n"
                           "  which specifies the storage location of the image to be decoded. A storage location is\n"
                           "  specified as: 'image/folder'. Where: 'image' corresponds to the name of the image to be\n" 
                           "  decoded and 'folder' the name of its corresponding folder. For example:\n" 
                           "  <decode this_image/in_this_folder> would decode the image named 'this_image' residing\n"
                           "  in the folder named 'in_this_folder'. Note that if the folder is omitted from the\n" 
                           "  specification a utility local default is applied. For example, if the default folder\n"
                           "  was 'in_this_folder' the command <decode this_image> would have the same effect as the\n" 
                           "  previous example.  Note: Both image and folder can be no longer then 63 characters.\n"
                           "\n";

void Editor::Decode::help(int argc, const char** argv, int qualifier)   
 {
 
 printf(HELP);
 
 return;
 }
    
