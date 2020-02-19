
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
 
#ifndef DSID_INTERFACE
#define DSID_INTERFACE

#include "dvi/Version.hh"
#include "net/ipv4/Address.hh"
#include "dsi/Client.hh"

namespace DSID {

class Interface {
public:
  Interface(const char* interface, bool=true);      
public: 
  Interface(Interface&) = delete;
public:
 ~Interface() {}
public:
  bool valid(unsigned) const;
public:
  const DSI::Set& servers() {return _client.servers();} 
public:
  bool probe(const DSI::Location&, DSI::Location& location, IPV4::Address&, DVI::Version&);
public: 
  unsigned lookup(const char* name);
  unsigned assign(const char* name);
  unsigned remove(const char* name);
  unsigned assigned();
public:
  const char* find(unsigned id, char buffer[], int length);
public:
  void restart();
  void shutdown();
private:
  DSI::Client _client;
};
                 
}

#endif

