
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
 
#ifndef DSM_INTERFACE
#define DSM_INTERFACE

#include "dsi/Location.hh"
#include "dsi/LocationSet.hh"
#include "daq/Location.hh"
#include "daq/Path.hh"

#include "dsm/Client.hh"
#include "dsm/Exception.hh"

namespace DSM {

namespace Request {class Get; class Set; class Reset;} // Forward links...
 
class Interface : public Client {
public:                         
  Interface(const char* partition) throw(Exception);  
public: 
  Interface()                 = delete;
  Interface(const Interface&) = delete;
public:
 ~Interface();
public:
  const DSI::LocationSet& servers() {return (const DSI::LocationSet&)_servers;} 
public: // Get the value of a preference for a specified server...
  bool partition(   const DSI::Location&, char* name,                 uint8_t& modified, bool current=true); 
  bool mountpoint(  const DSI::Location&, char* value,                uint8_t& modified, bool current=true);
  bool releasepoint(const DSI::Location&, char* value,                uint8_t& modified, bool current=true);
  bool role(        const DSI::Location&, char* value,                uint8_t& modified, bool current=true);
  bool path(        const DSI::Location&, DAQ::Path     interfaces[], uint8_t& modified, bool current=true);
  bool source(      const DSI::Location&, DAQ::Location interfaces[], uint8_t& modified, bool current=true);
public: // Set the value of a preference for a given set of servers...
  bool partition(   const DSI::LocationSet&, const char* name,                 DSI::LocationSet& missing);   
  bool mountpoint(  const DSI::LocationSet&, const char* name,                 DSI::LocationSet& missing); 
  bool releasepoint(const DSI::LocationSet&, const char* name,                 DSI::LocationSet& missing); 
  bool role(        const DSI::LocationSet&, const char* name,                 DSI::LocationSet& missing); 
  bool path(        const DSI::LocationSet&, const DAQ::Path interfaces[],     DSI::LocationSet& missing); 
  bool source(      const DSI::LocationSet&, const DAQ::Location interfaces[], DSI::LocationSet& missing); 
public: // Reset the value of a preference for a given set of servers...
  bool partition(   const DSI::LocationSet&, DSI::LocationSet& missing);   
  bool mountpoint(  const DSI::LocationSet&, DSI::LocationSet& missing); 
  bool releasepoint(const DSI::LocationSet&, DSI::LocationSet& missing); 
  bool role(        const DSI::LocationSet&, DSI::LocationSet& missing); 
  bool path(        const DSI::LocationSet&, DSI::LocationSet& missing);   
  bool source(      const DSI::LocationSet&, DSI::LocationSet& missing);     
private: 
  bool _wait(const Request::Get&,   char* name, uint8_t& modified); 
  bool _wait(const Request::Set&,   DSI::LocationSet& missing); 
  bool _wait(const Request::Reset&, DSI::LocationSet& missing); 
  };

}

#endif

