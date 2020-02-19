/*
** ++
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
**	000 - January 09, 2007
**
**  Revision History:
**	None.
**
** --
*/
 
#ifndef IPV4_TESTHANDLER
#define IPV4_TESTHANDLER

#include "net/ipv4/XSocket.hh"
#include "net/ipv4/Interface.hh"
#include "net/ipv4/Handler.hh"
#include "net/ipv4/Address.hh"

namespace IPV4 {

class TestHandler : public Handler {
public:
  TestHandler()                   = delete;
  TestHandler(const TestHandler&) = delete;
public:
  TestHandler(Interface&);
public:
 ~TestHandler() {delete[] _pattern;}
public:
  const Address& address() {return _socket.address;}
public:
  void wait();
public:
  bool process(const char* payload, int length, const Address&);
private:
  XSocket   _socket;
  uint16_t* _pattern;  
};

}

#endif
