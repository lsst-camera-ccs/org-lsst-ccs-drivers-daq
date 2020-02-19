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
**	000 - , 2007
**
**  Revision History:
**	None.
**
** --
*/
 
#ifndef IPV4_NEWSOCKET
#define IPV4_NEWSOCKET

#include "net/ipv4/Address.hh"
#include "net/ipv4/Interface.hh"
#include "net/ipv4/Exception.hh"
#include "net/ipv4/Handler.hh"

#include "net/rce/Sender.hh" 
#include "net/rce/Receiver.hh" 

namespace IPV4 {

class NewSocket {
public: 
  NewSocket(const Interface&, uint16_t port)       throw(Exception);  
  NewSocket(const Interface&, uint16_t port, bool) throw(Exception);  
public:
 ~NewSocket();
public:
  Address address;
public:  
 void sendTo(const Address& destination, const void* content, unsigned sizeof_content)  {_sender.postTo(destination, content, sizeof_content);}
public:  
  int  recv(void* payload, int sizeof_payload, Address& source)                         {return _receiver.recv(payload, sizeof_payload, source);}   
  void wait(Handler* handler)                                                           {_receiver.wait(handler);}
private:
  RCE::Sender   _sender;
  RCE::Receiver _receiver;    
};

}

#endif