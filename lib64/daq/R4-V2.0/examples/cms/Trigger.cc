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

#include "Trigger.hh"

#define INVALID        -1 // invalid opcode...
#define DEFAULT_OPCODE  1 // default opcode...

static int32_t _decode(const char* string);

static const char COMMAND[] = "trigger";

using namespace CMS;

/*
** ++
**
**
** --
*/

Editor::Trigger::Trigger(Camera& camera, const char* folder) :
 Command(COMMAND), 
 _camera(camera),
 _default(folder)
 { 
 
 }

/*
** ++
**
**
** --
*/

#define IMAGE      argv[0]
#define ANNOTATION argv[1]
#define OPCODE     argv[2]

#include "ims/Path.hh"
#include "ims/ImageMetadata.hh"
#include "cms/Exception.hh"
 
static const char SUCCESS[]        = "Triggered using opcode %d the image named %s at %s\n";
static const char FAILED[]         = "Failed to trigger using opcode %d the image to be named %s (%s)\n";
static const char INVALID_OPCODE[] = "The second argument is not a valid opcode\n";
static const char NOT_VALID[]      = "% is not a valid image specifier (first argument must be of the form: 'folder-name/image-name')\n";

void Editor::Trigger::process(int argc, const char** argv, int dc)   
 {

 if(!argc) return;

 IMS::Path path(IMAGE, _default);
 
 if(!path) {printf(NOT_VALID, IMAGE); return;}
 
 const char* annotation = argc >= 2 ? ANNOTATION      : (const char*)0;
 int32_t     opcode     = argc >  2 ? _decode(OPCODE) : DEFAULT_OPCODE;
 
 if(opcode == INVALID) {printf(INVALID_OPCODE); return;}
 
 IMS::ImageMetadata metadata(path.image(), path.folder(), _camera.sources(), opcode, annotation); 

 int32_t error = _camera.trigger(metadata);
 
 if(!error)
   printf(SUCCESS, opcode,  IMAGE, metadata.timestamp().decode());
 else
  printf(FAILED, opcode, IMAGE, CMS::Exception::decode(error));

 return; 
 }
  
/*
** ++
**
**
** --
*/

static const char DECODE[] = "%d";
                           
int32_t _decode(const char* string)
{

int32_t opcode;

return sscanf(string, DECODE, &opcode) == 1 ? opcode : INVALID;
}
  
/*
** ++
**
**
** --
*/

static const char HELP[] = "\n"
                           "  Acquire (trigger) and store an image. The command takes up to three arguments with the first argument\n"
                           "  required and the remaining two optional. The first argument specifies the storage location for the\n"
                           "  acquired and stored image. That argument takes the the form of: 'folder/image'. Where: 'image'\n" 
                           "  corresponds to the name to be assigned to the acquired image and 'folder' names the folder to contain\n"
                           "  the acquired image. For example: <trigger to_this_folder/my_image> triggers an image named 'my_image'\n"
                           "  and stores the acquired image in the folder named 'to this_folder'. If a folder name is omitted from\n" 
                           "  the specification a utility local default is applied. For example, assuming the default folder was\n" 
                           "  'to_this_folder' the command <trigger my_image> would have the same effect as the previous example.\n" 
                           "  Note: Neither image or folder names can be longer then sixty-three (63) characters.\n" 
                           "  The second argument is user defined string annotating the acquired image. That string may be no longer\n"
                           "  than sixty (63) characters. if this argument is omitted the annotation is a NULL (zero length) string.\n"
                           "  The third argument identifies the 'main' program executed in response to the trigger request. The argument\n"
                           "  is a small enumeration which may vary from  zero (0) to 15. If the argument is omitted a default main\n" 
                           "  (currently one (1)) will be triggered.\n"                                
                           "\n";
                                                      
void Editor::Trigger::help(int argc, const char** argv, int qualifier)   
 {
 
 printf(HELP);
 
 return;
 }
 
