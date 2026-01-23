
#include <stdio.h>

#include "Shell.hh"

#include "Read.hh"
#include "Write.hh"
#include "Reset.hh"
#include "ResetLink.hh"

using namespace RMS;

/*
** ++
**
**
** --
*/

Access::Shell::Shell(Client& client) :
  Utility::Shell(client)
{
  insert(new Access::Read(_client));
  insert(new Access::Write(_client));
  insert(new Access::Reset(_client));
  insert(new Access::ResetLink(_client));
}

/*
** ++
**
**
** --
*/

static const char HEADER[] =

" This utility is used to access the REB registers through RMS.\n"
" The following commands are available:\n";

#define INDENT (1+2)

void Access::Shell::announce()
 {

 printf(HEADER);

 dump_commands(INDENT);

 return;
 }

