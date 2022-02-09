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
#include "dsi/editor/Quit.hh"
#include "dsi/editor/Probe.hh"
#include "dsi/editor/Version.hh"

#include "List.hh"
#include "Delete.hh"
#include "Inspect.hh"
#include "Capacity.hh"
#include "Remaining.hh"
#include "Insert.hh"
#include "Remove.hh"
#include "Read.hh"
#include "Decode.hh"
#include "Compare.hh"
#include "ims/Shell.hh"

static const char ALL[]      = "all";
static const char ALL_HELP[] = "delete all images in the corresponding folder";

using namespace IMS;

/*
** ++
**
**
** --
*/

Shell::Shell(Store& store, const char* folder) :
 OSA::Shell(new DSI::Editor::Quit()),
 _store(store)
 {
 
 insert(new DSI::Editor::Probe(    store));
 insert(new DSI::Editor::Version());

 insert(new IMS::Editor::Insert(   store.catalog));
 insert(new IMS::Editor::Remove(   store.catalog));
 insert(new IMS::Editor::Capacity( store));
 insert(new IMS::Editor::Remaining(store));
 insert(new IMS::Editor::List(     store));
 insert(new IMS::Editor::Inspect(  store, folder));
 insert(new IMS::Editor::Delete(   store, folder));
 insert(new IMS::Editor::Read(     store, folder));
 insert(new IMS::Editor::Decode(   store, folder));
 insert(new IMS::Editor::Compare(  store, folder));
 
 insert(new OSA::Qualifier(ALL, 1, ALL_HELP));

 }

/*
** ++
**
**
** --
*/

static const char HEADER[] = 

" This utility is used to manage and configure the storage system.\n" 
" Input to the editor is of the form: <command> argument argument ... argument.\n" 
" The following commands and qualifiers are available:\n"; 

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
 
 printf(HEADER);

 dump_commands(INDENT);
 
 dump_qualifiers(INDENT);

 printf(TRAILER);

 return;
 }

