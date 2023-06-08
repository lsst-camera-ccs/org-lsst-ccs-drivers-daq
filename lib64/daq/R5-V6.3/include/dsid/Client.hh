
/*
**  Package:
**	
**
**  Abstract:
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
  
#ifndef DSID_CLIENT
#define DSID_CLIENT

#include "dsi/Client.hh"

namespace DSID {

class Client : public DSI::Client {
public: 
  static const char* interface();
public:
  Client(                       unsigned tmo=DSI::Client::TIMEOUT);     
  Client(const char* interface, unsigned tmo=DSI::Client::TIMEOUT);     
public:
  Client(const Client&) = delete;     
public:
 ~Client() {}
public:  
  const char* encode(uint8_t index, char buffer[]);
public:
  bool valid(unsigned) const;
public: 
  uint8_t lookup(const char* name);
  uint8_t assign(const char* name);
  uint8_t remove(const char* name); 
public:  
  uint32_t assigned();
public:
  const char* partition(uint8_t id, char* buffer);
};
                 
}

#endif

