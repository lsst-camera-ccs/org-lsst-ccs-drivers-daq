
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
 
#ifndef DSI_FRAME
#define DSI_FRAME

#include <stdint.h>

#include "net/ipv4/Address.hh"
#include "net/Endian.hh"
#include "dsi/Set.hh"

namespace DSI {

class __attribute__((__packed__)) Frame {
public:
  enum Function : uint8_t {F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24, F25, F26, F27, LOOKUP=28, PROBE=29, RESTART=30, SHUTDOWN=31, UNDEFINED};
public:
  Frame();  
  Frame(enum Function);
  Frame(enum Function, const Set&);  
  Frame(const Frame&);
public:
 ~Frame() {} 
public:  
  Frame& operator=(const Frame&);
public:
  bool valid()                     const;
  bool valid(const Frame* request) const;  
public: 
  uint32_t magic()    const {return Net::Endian::local(_magic);}
  uint32_t sequence() const {return Net::Endian::local(_sequence);}
  int32_t  status()   const {return Net::Endian::local(_status);}
  uint8_t  function() const {return _function;}
  uint8_t  server()   const {return _server;}
public:
  const IPV4::Address& source() const {return _source;}
public:
  const Set& accept() const {return _accept;}
public: 
  void set(int32_t status) {_status = Net::Endian::wire(status);}
  void set(uint8_t server) {if(_accept.has(server)) _server = server;}
public:
  void dump(int indent=0) const;
private:
  friend class Socket;
  friend class Client;
private:
  void _sequenceIs(uint32_t sequence) {_sequence = Net::Endian::wire(sequence);}
private:
  enum  {MAGIC = 0xABBABABE};    
private:
  uint32_t      _magic; 
  uint32_t      _sequence;
  int32_t       _status;   
  uint8_t       _server;  
  uint8_t       _function; 
  uint16_t      _pad; 
  IPV4::Address _source;
  Set           _accept;
  };

}

#endif

