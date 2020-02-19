
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
  static const char* interface();
public:
  static unsigned lookup(const char* name, const char* interface) throw(Exception);
  static unsigned lookup(const char* name)                        throw(Exception);
public: 
  Client()        = delete;
  Client(Client&) = delete;
public:
  Client(Services::Id, unsigned partition)                         throw(Exception);  
  Client(Services::Id, unsigned partition, const char* interface)  throw(Exception);  
  Client(Services::Id)                                             throw(Exception);  
  Client(Services::Id, const char* interface)                      throw(Exception);  
public:
 ~Client() {}
public:
  unsigned     pid() const {return _pid;}
  Services::Id sid() const {return _sid;}
private:
  const char*  _interface;
  unsigned     _pid;
  Services::Id _sid;
};
                 
}

#endif

