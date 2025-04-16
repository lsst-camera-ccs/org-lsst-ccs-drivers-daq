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

#include <csignal>

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
  Socket(const Interface&, bool is_service);
  Socket(const Interface&, uint16_t port, bool is_service);
  Socket(const Interface&,                unsigned tmo, bool is_service);
  Socket(const Interface&, uint16_t port, unsigned tmo, bool is_service);
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
  void _join(const Address& group) {__join(group);}
  void _drop(const Address& group) {__drop(group);}
private:  
  Datagram* _fill(Datagram*); 
private:
  void _set(int option, int value);
  void _set();
  void _set(int option);
  void _clear();
  void _clear(int option);
  void _setIp(int option, int value);
  void _clearIp(int option);
private:    
  unsigned _get(int option);
private:
  void _set_loopback();
  void _set_outgoing();
  void _set_options();
  void _set_defaults();
private:
  virtual size_t _header(const uint64_t opaque_header[]) const = 0;
private:
  EmptyBuffer _default;
  Pool*       _pool;
  Buffer*     _payload;
  Counters    _counters;
private:
  void _setupSignalHandling();
  static void _signalHandler(int signum);
  static volatile sig_atomic_t _shutdown;
};
  
}

#endif
