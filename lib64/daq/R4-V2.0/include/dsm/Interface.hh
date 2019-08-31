
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

#include "dsi/LocationSet.hh"
#include "daq/Interface.hh"

#include "dsm/Client.hh"
#include "dsm/Exception.hh"

#include "dsm/Partition.hh"
#include "dsm/Role.hh"
#include "dsm/Mode.hh"
#include "dsm/Mountpoint.hh"
#include "dsm/Releasepoint.hh"
#include "dsm/Path.hh"
#include "dsm/Source.hh"

#include "dsm/SourceProcessor.hh"
#include "dsm/PartitionProcessor.hh"
#include "dsm/RoleProcessor.hh"
#include "dsm/PathProcessor.hh"
#include "dsm/ModeProcessor.hh"
#include "dsm/ReleasepointProcessor.hh"
#include "dsm/MountpointProcessor.hh"

namespace DSM {
 
class Interface {
public:
  enum State : bool {CURRENT=true, PENDING=false};
public:
  static unsigned lookup(const char* name, const char* interface) throw(Exception);
  static unsigned lookup(const char* name)                        throw(Exception);
public:                         
  Interface(unsigned partition)                         throw(Exception);  
  Interface(unsigned partition, const char* interface)  throw(Exception);  
  Interface()                                           throw(Exception);  
  Interface(const char* interface)                      throw(Exception);  
public: 
  Interface(const Interface&) = delete;
public:
 ~Interface();
public:
  bool set(const Partition&, const DSI::LocationSet&, DSI::Set& missing);
  bool set(const Role&,      const DSI::LocationSet&, DSI::Set& missing);
public:  
  bool set(const Mode&,                               DSI::Set& missing);
  bool set(const Mountpoint&,                         DSI::Set& missing);
  bool set(const Releasepoint&,                       DSI::Set& missing);
public:  
  int32_t set(const Path&,                          const DSI::Location&);
  int32_t set(const Path&,   DAQ::Interface::Index, const DSI::Location&);
  int32_t set(const Source&, DAQ::Interface::Index, const DSI::Location&);
  int32_t set(const Source&,                        const DSI::Location&);
public:  
  bool get(Interface::State, PartitionProcessor&,    DSI::Set& missing);  
  bool get(Interface::State, ModeProcessor&,         DSI::Set& missing); 
  bool get(Interface::State, PathProcessor&,         DSI::Set& missing);
  bool get(Interface::State, SourceProcessor&,       DSI::Set& missing);
  bool get(Interface::State, RoleProcessor&,         DSI::Set& missing);
  bool get(Interface::State, MountpointProcessor&,   DSI::Set& missing);
  bool get(Interface::State, ReleasepointProcessor&, DSI::Set& missing);
public:
  const DSI::Set& servers() {return _client.servers();} 
public:
  bool probe(const DSI::Location&, DSI::Location&, IPV4::Address&, DVI::Version&);
public:
  void restart();
  void shutdown();
private:
  Client _client;  
  };

}

#endif

