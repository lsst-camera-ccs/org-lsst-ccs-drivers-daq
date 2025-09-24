
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

#include "dsm/Client.hh"
#include "daq/LocationSet.hh"
#include "emu/PlayList.hh"

namespace EMU {

class Client : public DSM::Client {
public: 
  Client(const char* partition);    
  Client(uint8_t     partition);     
public:                      
  Client(const char* partition, const char* interface);    
  Client(uint8_t     partition, const char* interface);     
public:  
  Client()              = delete; 
  Client(const Client&) = delete;
public:
 ~Client() {}
public: 
  const DAQ::LocationSet& emulators() const {return _emulators;}
public:
  bool play(const PlayList&, bool repeat=false); 
  bool halt();
  bool stop();
  bool restart(); 
  bool trigger(uint8_t opcode);
public:  
  const char* encode(uint8_t index, char buffer[]);
private:
  DAQ::LocationSet _emulators;
  };

}

#endif

