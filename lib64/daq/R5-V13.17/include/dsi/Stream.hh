
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
 
#ifndef DSI_STREAM
#define DSI_STREAM

#include "net/ipv4/Datagram.hh"
#include "net/ipv4/Exception.hh"

#include "dsi/Socket.hh"
#include "dsi/StreamAddress.hh"

namespace DSI {

class Stream : public Socket {
public:
  enum : uint64_t {TIMEOUT = 40}; // Default wait timeout in milliseconds (40 ms)
public:
  Stream(const StreamAddress& stream, const char* interface, uint64_t tmo=TIMEOUT);     
public:
  Stream()              = delete;
  Stream(const Stream&) = delete;
public:
 ~Stream();
public: 
  const StreamAddress& stream() const {return _stream;}
public:
  void set_timeout(uint64_t ms) {_set_timeout(ms);}
  __attribute__((deprecated("use set_timeout(uint64_t ms) - argument is now milliseconds, not 10ms tics")))
  void set(unsigned tmo=0) {_set_timeout((uint64_t)tmo * 10);}
private:
  StreamAddress _stream;
  }; 
                 
}

#endif

