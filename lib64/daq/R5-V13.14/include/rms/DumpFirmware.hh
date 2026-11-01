
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
 
#ifndef PPS_DUMPFIRMWARE
#define PPS_DUMPFIRMWARE

#include "dsi/Location.hh"
#include "dsi/Request.hh"
#include "dsi/LocationSet.hh"
#include "rms/Client.hh"
#include "rms/Firmware.hh"

namespace RMS { 

class DumpFirmware  {
public:
  DumpFirmware(Client&, const DSI::LocationSet&);
public:
  DumpFirmware()                    = delete;
  DumpFirmware(const DumpFirmware&) = delete;
public:
 ~DumpFirmware() {}
public:
  bool next(DAQ::Location&, Firmware&);
private:
  DSM::Client&     _client;
  DSI::Request     _request;
  DSI::LocationSet _remaining;
};

}

#endif


