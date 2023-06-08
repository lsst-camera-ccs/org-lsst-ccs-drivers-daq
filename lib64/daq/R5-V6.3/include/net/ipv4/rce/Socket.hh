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
 
#ifndef IPV4_SOCKET
#define IPV4_SOCKET

#include "net/ipv4/Buffer.hh"
#include "net/ipv4/EmptyBuffer.hh"
#include "net/ipv4/SocketBase.hh"
#include "net/ipv4/Counters.hh"
#include "net/ipv4/Address.hh"
#include "net/ipv4/Datagram.hh"
#include "net/ipv4/Rundown.hh"
#include "net/ipv4/Interface.hh"
#include "net/ipv4/Exception.hh"
#include "net/ipv4/JoinException.hh"

namespace IPV4 {

class Pool;      // Forward link...
class IbSocket;  // Forward link...
class ObSocket;  // Forward link...

class Socket : public SocketBase {
public: 
  Socket(const Interface&)                              throw(Exception);  
  Socket(const Interface&, uint16_t port)               throw(Exception);  
  Socket(const Interface&,                unsigned tmo) throw(Exception); 
  Socket(const Interface&, uint16_t port, unsigned tmo) throw(Exception);  
public:
  Socket()              = delete;
  Socket(const Socket&) = delete; 
public:
  virtual ~Socket();
public:  
  const Counters& counters() const;
protected: 
  Datagram* _wait();
  void      _deallocate(Datagram*);  
  bool      _send(Datagram&);
  bool      _post(Datagram&, Rundown* rundown=(Rundown*)0); 
  void      _use(Buffer& buffer) {_use(&buffer);}
  void      _use(Buffer*);
  void      _use();
  void      _flush(void* payload, unsigned length);
  unsigned  _remaining() const; 
  void      _set(unsigned timout) {_timout = timout;}
protected: 
  virtual void _join(const Address&) throw(JoinException) final;    
  virtual void _drop(const Address&) throw(JoinException) final;  
private:
  friend class AcceptOne;
private:
  Buffer* _get();
private:
  virtual size_t _header(const uint64_t opaque_header[]) const = 0;
private:
  EmptyBuffer _default;
  unsigned    _timout;
  Buffer*     _payload;
  Pool*       _pool;
  IbSocket*   _inbound;
  ObSocket*   _outbound;
};

}

#endif