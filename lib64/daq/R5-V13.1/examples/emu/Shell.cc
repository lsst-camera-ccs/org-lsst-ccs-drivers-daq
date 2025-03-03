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

#include "Generate.hh"
#include "Add.hh"
#include "Dump.hh"

#include "Shell.hh"

using namespace EMU;

/*
** ++
**
**
** --
*/

Shell::Shell(IMS::Store& store, const char* folder, PlayList* playlist) :
 IMS::Shell(store, folder)
 {
  
 insert(new EMU::Editor::Dump(    store,         playlist));
 insert(new EMU::Editor::Add(     store, folder, playlist));
 insert(new EMU::Editor::Generate(store, folder));

 }

/*
** ++
**
**
** --
*/

static const char HEADER[] = 

" This utility is used to manage a playlist, access images and as well to manage and configure\n" 
" the storage system. Input to the editor is of the form: <command> argument argument ... argument.\n" 
" The following commands are available:\n"; 

static const char TRAILER[] = 

" Commands may have one or more arguments all of which are separated by white space.\n" 
" Note that commands are case sensitive and must be typed in their entirety. For\n"
" example: <help> is a valid command, but not <HELP> or <h>. To obtain help on any one\n" 
" command type <help> followed by the corresponding command. For example: <help quit>\n"
" provides help on the quit command. To repeat this announcement type <help> with no\n" 
" arguments. To exit the editor type <quit> at the prompt.\n\n";

#define INDENT (1+2)

void Shell::announce() 
 {

 dump_qualifiers(INDENT);
 
 printf(HEADER);

 dump_commands(INDENT);
 
 printf(TRAILER);

 return;
 }

