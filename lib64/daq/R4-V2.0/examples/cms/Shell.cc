/*
** ++
**  Package:
**	
**
**  Abstract:
**      
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

#include "osa/Qualifier.hh"

#include "Trigger.hh"
#include "Sequence.hh"
#include "Dump.hh"

#include "Shell.hh"

static const char EMIT_HELP[] = "...";
static const char EMIT[]      = "emit";

static const char ONE_HELP[] = "...";
static const char ONE[]      = "one";

static const char TWO_HELP[] = "...";
static const char TWO[]      = "two";

using namespace CMS;

/*
** ++
**
**
** --
*/

Editor::Shell::Shell(Camera& camera, const char* folder) :
 IMS::Shell(camera.store(), folder),
 _camera(camera)
 {

 insert(new Dump(    _camera));
 insert(new Trigger( _camera, folder));
 insert(new Sequence(_camera));

 insert(new OSA::Qualifier(EMIT, 1, EMIT_HELP));
 insert(new OSA::Qualifier(ONE,  1, ONE_HELP));
 insert(new OSA::Qualifier(TWO,  2, ONE_HELP));

 }

/*
** ++
**
**
** --
*/

static const char HEADER2[] = 

" This utility is used to....\n" 
" Input to the editor is of the form: <command> argument. Where:\n" 
" argument corresponds to the name of a key either accessed or modified.\n"
" The following commands are available:\n"; 

static const char TRAILER[] = 

" Commands may have one or more arguments all of which are separated by white space. Note\n" 
" that commands are case sensitive and must be typed in their entirety. For\n"
" example: <help> is a valid command, but not <HELP> or <h>. To obtain help on any one command\n" 
" type <help> followed by the corresponding command. For example: <help assign> provides help\n"
" on the assign command. To repeat this announcement type <help> with no arguments.\n" 
" To exit the editor type <quit> at the prompt.\n\n";

#define INDENT (1+2)

void Editor::Shell::announce() 
 {
 
 //printf(HEADER1);

 dump_qualifiers(INDENT);
 
 printf(HEADER2);

 dump_commands(INDENT);
 
 printf(TRAILER);

 return;
 }

