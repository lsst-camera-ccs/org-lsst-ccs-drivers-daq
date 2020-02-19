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
#include "dsm/Interface.hh"

#include "Quit.hh"
#include "Restart.hh"
#include "Stop.hh"
#include "Probe.hh"

#include "dsm/Editor.hh"

using namespace DSM;

static const char INDEX_0[]    = "0";
static const char INDEX_1[]    = "1";
static const char INDEX_2[]    = "2";

static const char INDEX_HELP[] = "The Index may vary from 0 (zero) to 2 (two)";

static const char VALUE_CURRENT[]    = "current";
static const char VALUE_PENDING[]    = "pending";

static const char CURRENT_HELP[] = "may be either 'current' or 'pending'";

/*
** ++
**
**
** --
*/

Editor::Editor(Interface& interface) :
 OSA::Editor(new Quit(*this)),
 _version(),
 _interface(interface)
 {

 insert(new Probe(  _interface, *this));
 insert(new Restart(_interface, *this));
 insert(new Stop(   _interface, *this));

 insert(new OSA::Qualifier(INDEX_0, Index::ZERO, INDEX_HELP));
 insert(new OSA::Qualifier(INDEX_1, Index::ONE,  INDEX_HELP));
 insert(new OSA::Qualifier(INDEX_2, Index::TWO,  INDEX_HELP));

 insert(new OSA::Qualifier(VALUE_CURRENT, Value::CURRENT, CURRENT_HELP));
 insert(new OSA::Qualifier(VALUE_PENDING, Value::PENDING, CURRENT_HELP));

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
" If a (necessary) qualifier for a command is omitted the command, by default is qualified by the\n"
" constant: 'interfaces'. For example, <assign/role> defines the 'role' for a specified set of RCEs,\n"
" while <assign> defines the 'interfaces' for the same set of RCEs. The allowed commands are:\n"; 

static const char TRAILER[] = 

" Commands may have one or more arguments all of which are separated by white space. Note\n" 
" that both command and qualifier are case sensitive and must be typed in their entirety. For\n"
" example: <help> is a valid command, but not <HELP> or <h>. To obtain help on any one command\n" 
" type <help> followed by the corresponding command. For example: <help assign> provides help\n"
" on the assign command. To repeat this announcement type <help> with no arguments.\n" 
" To exit the editor type <quit> at the prompt.\n\n";

#define INDENT (1+2)

void Editor::announce() 
 {
 
 printf(HEADER);

 dump_commands(INDENT);
 
 printf(TRAILER);

 return;
 }

/*
** ++
**
**
** --
*/

static const char THROW[] = "%s cannot access the following locations...\n";

void Editor::_throw(const char* command, DSI::LocationSet& missing)
 {
 
 printf(THROW, command); missing.dump();
 
 return;
 }





