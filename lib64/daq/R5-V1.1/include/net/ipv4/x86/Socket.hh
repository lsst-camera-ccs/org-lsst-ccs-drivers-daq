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
#include "net/ipv4/Datagram.hh"
#include "net/ipv4/Address.hh"
#include "net/ipv4/Counters.hh"
#include "net/ipv4/Interface.hh"
#include "net/ipv4/Exception.hh"
#include "net/ipv4/SocketBase.hh"
#include "net/ipv4/SetException.hh"
#include "net/ipv4/JoinException.hh"
#include "net/ipv4/Rundown.hh"

namespace IPV4 {

class Pool; // Forward link...

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
  const Counters& counters() const {return _counters;}
protected:
  void      _use(Buffer& buffer) {_payload = &buffer;} 
  void      _use(Buffer* buffer) {_payload = buffer;}
  void      _use()               {_payload = &_default;} 
  Datagram* _wait();
  void      _deallocate(Datagram*); 
  bool      _send(Datagram& datagram) {return _post(datagram);} 
  bool      _post(Datagram&, Rundown* rundown=(Rundown*)0);
  void      _flush(void* payload, unsigned length) {return;}
  unsigned  _remaining() const; 
  void      _set(unsigned tmo);
protected:   
  void _join(const Address& group) throw(JoinException) {__join(group);}
  void _drop(const Address& group) throw(JoinException) {__drop(group);}
private:  
  Datagram* _fill(Datagram*); 
private:
  void _set(int option, int value)   throw(SetException);
  void _set()                        throw(SetException);
  void _set(int option)              throw(SetException);
  void _clear()                      throw(SetException); 
  void _clear(int option)            throw(SetException); 
  void _setIp(int option, int value) throw(SetException);   
  void _clearIp(int option)          throw(SetException);
private:    
  unsigned _get(int option) throw(SetException);  
private:
  void _set_loopback() throw(SetException);      
  void _set_outgoing() throw(SetException); 
  void _set_options()  throw(SetException); 
  void _set_defaults() throw(SetException); 
private:
  virtual size_t _header(const uint64_t opaque_header[]) const = 0;
private:
  EmptyBuffer _default;
  Pool*       _pool;
  Buffer*     _payload;
  Counters    _counters;
};
  
}

#endif
