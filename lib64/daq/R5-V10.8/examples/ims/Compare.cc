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

#include "Compare.hh"

static const char COMMAND[] = "compare";

static float _rate(uint64_t size, long interval);

using namespace IMS;

/*
** ++
**
**
** --
*/

Editor::Compare::Compare(Store& store, const char* folder) : 
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
#include "Comparator.hh"

#define IMAGE1 argv[0]
#define IMAGE2 argv[1]

static const char NOT_VALID[]    = "%s is not a valid image (first argument must be of the form: 'image-name/folder-name'). Is there a default folder?\n";
static const char LOOKUP_ERROR[] = "Cannot find '%s in the storage catalog\n";
static const char OPEN_ERROR[]   = "Cannot locate the metadata for '%s\n";
static const char COMPARATOR[]   = "Decoded and compared %llu byte(s) of data (matched %llu) in %ldus (%.0f Mbytes/second)\n"; 
                           
void Editor::Compare::process(int argc, const char** argv, int qualifier)   
 {
 
 if(!argc) return;

 Path path1(IMAGE1, _default);
 
 if(!path1) {printf(NOT_VALID, IMAGE1); return;}
 
 Path path2(argc == 2 ? IMAGE2 : IMAGE1, _default);
 
 if(!path2) {printf(NOT_VALID, IMAGE2); return;}
 
 Id id1 = _store.catalog.lookup(path1.image(), path1.folder());

 if(!id1) {printf(LOOKUP_ERROR, IMAGE1); return;}

 Id id2 = _store.catalog.lookup(path2.image(), path2.folder());

 if(!id2) {printf(LOOKUP_ERROR, IMAGE2); return;}

 Image image1(id1, _store);
 
 if(!image1) {printf(OPEN_ERROR, IMAGE1); return;}

 Comparator comparator(image1, id2, _store);
   
 OSA::Timer timer; timer.start(); 
   
 comparator.run();
 
 long duration = timer.stop();

 uint64_t processed = comparator.processed();
 
 printf(COMPARATOR, (long long unsigned)processed, (long long unsigned)comparator.matched(), duration, _rate(processed, duration));
   
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
                           "  Read, decode and (bitwise) compare  the contents of two images. This command takes\n" 
                           "  either one or two arguments where the arguments specify the storage location of\n"
                           "  the two images to be compared. A storage location is specified as: 'image/folder'.\n" 
                           "  Where: 'image' corresponds to the name of an image to be compared and 'folder'\n"
                           "  the name of its corresponding folder. For example:\n" 
                           "  <compare this_image/in_my_folder that_image/in_my_folder> compares the image named\n" 
                           " 'this_image' in the folder named 'in_my_folder' to 'that_image' in 'in_my_folder'.\n" 
                           "  If the second argument is omitted the comparison is against the first argument.\n" 
                           "  Note that if the folder is omitted from a specification a utility, local default\n"
                           "  is applied. For example, if the default folder was 'in_this_folder' the command\n" 
                           "  <compare this_image that_image> would have the same effect as the previous example.\n" 
                           "  Or, conversely <compare this_image> would compare 'this_image' from the default\n"
                           "  folder to itself. Note: Both image and folder can be no longer then 63 characters.\n"
                           "\n";

void Editor::Compare::help(int argc, const char** argv, int qualifier)   
 {
 
 printf(HELP);
 
 return;
 }
    
