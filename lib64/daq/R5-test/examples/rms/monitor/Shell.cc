
#include <stdio.h>

#include "Shell.hh"

#include "Service.hh"
#include "Driver.hh"
#include "Firmware.hh"

using namespace RMS;

/*
** ++
**
**
** --
*/

Monitor::Shell::Shell(Client& client) :
  Utility::Shell(client)
{
  insert(new Monitor::Service(_client));
  insert(new Monitor::Driver(_client));
  insert(new Monitor::Firmware(_client));
}

/*
** ++
**
**
** --
*/

static const char HEADER[] = 
" This utility is used to access RMS and PGP statistics.\n"
" The following commands are available:\n"; 

#define INDENT (1+2)

void Monitor::Shell::announce() 
 {
 
 printf(HEADER);

 dump_commands(INDENT);

 return;
 }

