
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
 
#ifndef EMU_CLIENT
#define EMU_CLIENT

#include "daq/LocationSet.hh"
#include "dsm/Client.hh"
#include "dsm/Exception.hh"
#include "emu/PlayList.hh"

namespace EMU {

class Client {
public:  
  Client()              = delete; 
  Client(const Client&) = delete;
public:                         
  Client(const char* partition, const char* interface) throw (DSM::Exception);     
  Client(unsigned    partition, const char* interface) throw (DSM::Exception);     
  Client(const char* partition)                        throw (DSM::Exception);    
public:
 ~Client() {}
public: 
  const DAQ::LocationSet& emulators() const {return _emulators;}
public:  
  bool probe(const DAQ::Location&, DSI::Location&, IPV4::Address&, DVI::Version&); 
public:
  bool play(const PlayList&, bool repeat=false);
public:  
  bool halt();
  bool stop();
  bool restart(); 
public:
  void trigger(int32_t opcode);
private:
  void _probe();  
private:
  DSM::Client      _client;
  DAQ::LocationSet _emulators;
  };

}

#endif

