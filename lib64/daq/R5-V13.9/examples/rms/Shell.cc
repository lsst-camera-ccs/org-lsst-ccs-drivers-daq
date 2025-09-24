
#include <stdio.h>

#include "osa/Qualifier.hh"

#include "Shell.hh"
#include "dsi/editor/Version.hh"
#include "dsi/editor/Quit.hh"
#include "dsi/editor/Probe.hh"
#include "dsi/editor/Restart.hh"

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
  insert(new DSI::Editor::Version());
  insert(new DSI::Editor::Probe(_client));
  insert(new DSI::Editor::Restart(_client, *this));
}

