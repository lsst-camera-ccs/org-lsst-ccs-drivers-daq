#include <stdio.h>
#include <stdlib.h>

#include "BadPixelCount.hh"

static const char COMMAND[] = "badpixel";

using namespace IMS;

/*
** ++
**
**
** --
*/

Editor::BadPixelCount::BadPixelCount(Store& store, const char* folder) : 
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
#include "BadPixel.hh"

#define IMAGE argv[0]

static const char NOT_VALID[]    = "%s is not a valid image (first argument must be of the form: 'folder-name/image-name'). Is there a default folder?\n";
static const char LOOKUP_ERROR[] = "Cannot find '%s in the storage catalog\n";
static const char OPEN_ERROR[]   = "Cannot locate the metadata for '%s\n";
static const char RANGE[]        = "Scanning with good pixel range [%d, %d]\n";
static const char DUMP_MODE[]    = "Dump mode: printing up to %u bad stripes with context\n";
static const char DUMP_ALL[]     = "Dump mode: printing all bad stripes with context\n";
static const char RESULT[]       = "Scanned %llu pixels in %ldus, found %llu bad pixels in %u bad stripes\n"; 
static const char NOT_SHOWN[]    = "  ... %u of %u bad stripes not shown (limit %u)\n";

#define DUMP_QUALIFIER 2

void Editor::BadPixelCount::process(int argc, const char** argv, int qualifier)   
 {
 
 if(!argc) return;
 
 int32_t  low  = BadPixel::DEFAULT_LOW;
 int32_t  high = BadPixel::DEFAULT_HIGH;
 unsigned max_dump = BadPixel::DEFAULT_MAX_DUMP;

 if(argc >= 3)
  {
  low  = atoi(argv[1]);
  high = atoi(argv[2]);
  }

 if(argc >= 4)
  max_dump = (unsigned)atoi(argv[3]);

 Path path(IMAGE, _default);
 
 if(!path) {printf(NOT_VALID, IMAGE); return;}
 
 Id bucket = _store.catalog.lookup(path.image(), path.folder());
 
 if(!bucket) {printf(LOOKUP_ERROR, IMAGE); return;}
 
 Image image(bucket, _store);
 
 if(!image) {printf(OPEN_ERROR, IMAGE); return;}

 Image::title(); image.synopsis();

 printf(RANGE, low, high);
 
 BadPixel decoder(image, low, high);

 if(qualifier == DUMP_QUALIFIER)
  {
  decoder.enable_dump(max_dump);
  if(max_dump)
   printf(DUMP_MODE, max_dump);
  else
   printf(DUMP_ALL);
  }
   
 OSA::Timer timer; timer.start(); 
   
 decoder.run();
 
 long duration = timer.stop();

 printf(RESULT, (long long unsigned)decoder.total(), duration,
        (long long unsigned)decoder.bad(), decoder.bad_stripes());

 if(qualifier == DUMP_QUALIFIER && max_dump && decoder.dumped() < decoder.bad_stripes())
  printf(NOT_SHOWN, decoder.bad_stripes() - decoder.dumped(),
         decoder.bad_stripes(), max_dump);

 decoder.dump();
   
 return; 
 }

/*
** ++
**
**
** --
*/

static const char HELP[] = "\n"
                           "  Scan the contents of a specified image for bad pixels. A pixel is considered bad\n"
                           "  if its decoded value falls outside the specified good range [low, high] (inclusive).\n"
                           "  This command takes one to four arguments:\n"
                           "    badpixel      <image> [low high]\n"
                           "    badpixel/dump <image> [low high [max]]\n"
                           "  Where: 'image' specifies the storage location of the image to be scanned, expressed\n" 
                           "  as 'folder-name/image-name'. If low and high are omitted the default good range of\n"
                           "  [10000, 240000] is used. For example:\n"
                           "    <badpixel this_image/in_this_folder 10000 50000>\n"
                           "  would scan the image for pixels outside the range [10000, 50000].\n"
                           "  The /dump qualifier prints each stripe containing bad pixels along with one good\n"
                           "  neighboring stripe on either side for context. Bad segment values are flagged with\n"
                           "  asterisks. The optional 'max' argument limits the number of stripes printed (default\n"
                           "  100, 0 for unlimited). For example:\n"
                           "    <badpixel/dump this_image/in_this_folder 10000 50000 50>\n"
                           "  would print the first 50 bad stripes with context.\n"
                           "  Note that if the folder is omitted from the image specification a utility local\n" 
                           "  default is applied.\n"
                           "\n";

void Editor::BadPixelCount::help(int argc, const char** argv, int qualifier)   
 {
 
 printf(HELP);
 
 return;
 }
    
