
#include <stdio.h>

#include "osa/Qualifier.hh"

#include "Shell.hh"
#include "dsi/editor/Quit.hh"
#include "dsi/editor/Probe.hh"

using namespace RMS;

/*
** ++
**
**
** --
*/

Utility::Shell::Shell(Client& client) :
  OSA::Shell(new DSI::Editor::Quit()),
  _client(client)
{
  insert(new DSI::Editor::Probe(_client));
}

