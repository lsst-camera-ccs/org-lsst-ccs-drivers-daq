
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
** --ps
*/
 
#ifndef DSM_CLIENT
#define DSM_CLIENT

#include "dsi/Client.hh"
#include "dsm/Services.hh"
#include "dsm/Exception.hh"

namespace DSM {

class Client : public DSI::Client {
public:
  Client(Services::Id, const char* partition); 
  Client(Services::Id, uint8_t     partition); 
public:
  Client(Services::Id, const char* partition, const char* interface); 
  Client(Services::Id, uint8_t     partition, const char* interface); 
public: 
  Client()        = delete;
  Client(Client&) = delete;
public:
 ~Client() {}
public: 
  Services::Id sid() const {return (Services::Id)service().offset();}
  uint8_t      pid() const {return service().block();}
private:
  uint8_t _lookup(const char* partition);
  uint8_t _lookup(const char* partition, const char* interface);
};
                 
}

#endif

