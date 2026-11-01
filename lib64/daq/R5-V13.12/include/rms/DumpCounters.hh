
/*
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
**	    000 - April 06, 2011
**
**  Revision History:
**	    None.
**
** --
*/
 
#ifndef PPS_DUMPCOUNTERS
#define PPS_DUMPCOUNTERS

#include "dsi/Location.hh"
#include "dsi/Request.hh"
#include "dsi/LocationSet.hh"
#include "pps/Client.hh"
#include "pps/Counters.hh"

namespace PPS { 

class DumpCounters  {
public:
  DumpCounters(Client&, const DSI::LocationSet&);
public:
  DumpCounters()                    = delete;
  DumpCounters(const DumpCounters&) = delete;
public:
 ~DumpCounters() {}
public:
  bool next(DSI::Location&, Counters&);
private:
  DSM::Client&     _client;
  DSI::Request     _request;
  DSI::LocationSet _remaining;
};

}

#endif


