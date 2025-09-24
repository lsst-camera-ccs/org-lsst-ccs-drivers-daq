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
#include "osa/TimeStamp.hh"

#include "Sequence.hh"

static int32_t _decode(const char* string);

static const char COMMAND[] = "sequence";

using namespace CMS;

/*
** ++
**
**
** --
*/

Editor::Sequence::Sequence(Camera& camera) :
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

#define OPCODE argv[0]
#define INVALID -1

static const char INVALID_OPCODE[] = "The command argument is not a valid opcode\n"; 
static const char SUCCESS[]        = "Sequenced using opcode %d at %s\n";
static const char FAILED[]         = "Failed to sequence using opcode %d\n";

void Editor::Sequence::process(int argc, const char** argv, int dc)   
 {

 if(!argc) return;

 int32_t opcode = _decode(OPCODE);
 
 if(opcode == INVALID) {printf(INVALID_OPCODE); return;}
 
 OSA::TimeStamp timestamp(false);

 if(_camera.sequence(opcode, timestamp))
  printf(SUCCESS, opcode, timestamp.decode());
 else
  printf(FAILED, opcode);

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
                              
void Editor::Sequence::help(int argc, const char** argv, int qualifier)   
 {
 
 printf(HELP);
 
 return;
 }
 
