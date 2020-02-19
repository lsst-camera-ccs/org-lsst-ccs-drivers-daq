
#include <stdio.h>

#include "osa/Qualifier.hh"

#include "Shell.hh"
#include "Quit.hh"
#include "Probe.hh"

using namespace RMS;

/*
** ++
**
**
** --
*/

Utility::Shell::Shell(Client& client) :
  OSA::Editor(new Quit()),
  _client(client)
{
  insert(new RMS::Utility::Probe(_client));
}

