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

Editor::Trigger::Trigger(Camera& camera) :
 Command(COMMAND), 
 _camera(camera) 
 { 
 
 }

/*
** ++
**
**
** --
*/

#define NAME       argv[0]
#define ANNOTATION argv[1]
#define OPCODE     argv[2]

#include "ims/ImageMetadata.hh"
#include "cms/Exception.hh"
 
static const char SUCCESS[]        = "Triggered using opcode %d the image named %s at %s\n";
static const char FAILED[]         = "Failed to trigger using opcode %d the image to be named %s (%s)\n";
static const char INVALID_OPCODE[] = "The second argument is not a valid opcode\n";

void Editor::Trigger::process(int argc, const char** argv, int dc)   
 {

 if(!argc) return;

 const char* annotation = argc >= 2 ? ANNOTATION      : (const char*)0;
 int32_t     opcode     = argc >  2 ? _decode(OPCODE) : DEFAULT_OPCODE;
 
 if(opcode == INVALID) {printf(INVALID_OPCODE); return;}
 
 IMS::ImageMetadata metadata(NAME, _camera.sources(), opcode, annotation); 

 int32_t error = _camera.trigger(metadata);
 
 if(!error)
   printf(SUCCESS, opcode,  NAME, metadata.timestamp().decode());
 else
  printf(FAILED, opcode, NAME, CMS::Exception::decode(error));

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

static const char HELP[] = "TBD\n";
                              
void Editor::Trigger::help(int argc, const char** argv, int qualifier)   
 {
 
 printf(HELP);
 
 return;
 }
 
