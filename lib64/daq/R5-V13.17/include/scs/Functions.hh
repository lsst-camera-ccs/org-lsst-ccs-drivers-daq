
#ifndef SCS_FUNCTIONS
#define SCS_FUNCTIONS

#include "dsi/Header.hh"

#define ADDRESS DSI::Header::F0  // Get server's IP address
#define TRIGGER DSI::Header::F4  // Issue synchronous command
#define STATS   DSI::Header::F5  // Return DTM stats

#endif

