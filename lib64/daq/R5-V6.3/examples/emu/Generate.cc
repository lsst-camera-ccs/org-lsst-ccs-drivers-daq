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

#include "Generate.hh"

static const char COMMAND[] = "generate";

static float _rate(uint64_t size, long interval);

using namespace EMU;

/*
** ++
**
**
** --
*/

Editor::Generate::Generate(IMS::Store& store, const char* folder) : 
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
#include "daq/All.hh"
#include "daq/ScienceSet.hh"
#include "daq/GuidingSet.hh"
#include "daq/WavefrontSet.hh"
#include "ims/Image.hh"
#include "ims/Exception.hh"
#include "ims/ImageMetadata.hh"
#include "ims/Path.hh"
#include "Generator.hh"

#define IMAGE  argv[0]
#define OPCODE int32_t(0)

static const char ANNOTATION[] = "1179648 samples/SCI of pseudo-random data";

static const char NOT_VALID[]       = "%s is not a valid image specifier (first argument must be of the form: 'image-name/folder-name'). Is there a default folder?\n";
static const char DUPLICATE_ERROR[] = "Cannot generate '%s' (Image already exists)\n";
static const char CREATE_ERROR[]    = "Image '%s'cannot be created (%s)\n";
static const char COMMIT[]          = "Committed %llu bytes of data in %ldus (%.0f Mbytes/second)\n"; 
                             
void Editor::Generate::process(int argc, const char** argv, int qualifier)   
 {
 
 if(!argc) return;
 
 IMS::Path path(IMAGE, _default);
 
 if(!path) {printf(NOT_VALID, IMAGE); return;}
 
 if(_store.catalog.probe(path.image(), path.folder())) {printf(DUPLICATE_ERROR, IMAGE); return;}

 DAQ::LocationSet some(argc-1, &argv[1]);
 DAQ::All         all;
    
 IMS::ImageMetadata metadata(path.image(), path.folder(), argc != 1 ? some : all, OPCODE, ANNOTATION);
  
 IMS::Image image(metadata, _store);
  
 if(!image) {printf(CREATE_ERROR, IMAGE, IMS::Exception::decode(image.error())); return;}

 Generator generator(image);
    
 OSA::Timer timer; timer.start(); 
 
 generator.run();

 long duration = timer.stop();

 IMS::Image::title(); image.synopsis();

 uint64_t size = generator.size();
 
 printf(COMMIT, (long long unsigned)size, duration, _rate(size, duration));
 
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
                           "  Generate a template image. The image emulates the entire FPA (both science and corner\n" 
                           "  rafts). Its data contents are a pseudo-random set of data. The command takes a single\n"
                           "  argument which specifies the storage location for the created template image. That\n" 
                           "  argument takes the the form of: 'folder/image'. Where: 'image' corresponds to the name\n" 
                           "  of the template to be created and 'folder' names the folder to contain the created template.\n"
                           "  For example: <generate in_my_folder/my_image> creates the template named 'my_image' and\n"
                           "  stores the resulting template to the folder named 'in_my_folder'. Note that if the folder\n"
                           "  is omitted from the specification a utility local default is applied. For example, if the\n"
                           "  default folder was 'in_this_folder' the command <generate my_image> would have the same\n" 
                           "  effect as the previous example. Note: Neither image or folder names can be no longer then\n"
                           "  sixty-three (63) characters.\n"
                           "\n";
                           
void Editor::Generate::help(int argc, const char** argv, int qualifier)   
 {
 
 printf(HELP);
 
 return;
 }
    
