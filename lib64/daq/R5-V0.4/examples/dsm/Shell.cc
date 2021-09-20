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

#include "dsi/editor/Version.hh"
#include "dsi/editor/Quit.hh"
#include "dsi/editor/Restart.hh"
#include "dsi/editor/Stop.hh"
#include "dsi/editor/Probe.hh"
#include "dsi/editor/Push.hh"
#include "dsi/editor/Empty.hh"

#include "Shell.hh"

using namespace DSM::Editor;

/*
** ++
**
**
** --
*/

Shell::Shell(Interface& interface) :
 OSA::Shell(new DSI::Editor::Quit()),
 _interface(interface),
 _stack(interface.servers(), interface.servers())
 {

 insert(new DSI::Editor::Version());
 insert(new DSI::Editor::Push(_stack));
 insert(new DSI::Editor::Empty(_stack));
 insert(new DSI::Editor::Probe(  interface));
 insert(new DSI::Editor::Restart(interface, *this));
 insert(new DSI::Editor::Stop(   interface, *this));

 }

/*
** ++
**
**
** --
*/

static const char HEADER[] = 

" Input to the editor is of the form: <command/qualifier> argument-1 argument-2... Where:\n" 
" /qualifier modifies (qualifies) the function of the command and corresponds to the name of a\n"
" constant either accessed or modified. It may take on any one of the six values defined above.\n"
" If a qualifier for an issued command is omitted the command will have a default qualifier.\n"
" For example, <set/role> defines the 'role' for a specified set of RCEs,\n"
" while <set> defines the 'interfaces' for the same set of RCEs. The allowed commands are:\n"; 

static const char TRAILER[] = 

" Commands may have one or more arguments all of which are separated by white space. Note\n" 
" that both command and qualifier are case sensitive and must be typed in their entirety. For\n"
" example: <help> is a valid command, but not <HELP> or <h>. To obtain help on any one command\n" 
" type <help> followed by the corresponding command. For example: <help set> provides help\n"
" for the set command. To repeat this announcement type <help> with no arguments.\n" 
" To exit the editor type <quit> at the prompt.\n\n";

#define INDENT (1+2)

void Shell::announce() 
 {
 
 DVI::Version version;
 
 printf(HEADER);

 dump_commands(INDENT);
 
 printf(TRAILER);

 return;
 }

