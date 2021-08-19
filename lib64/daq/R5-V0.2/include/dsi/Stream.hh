
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
  enum {TIMEOUT = 4}; // Default wait timeout in 10 ms tics (40 milliseconds)... 
public:
  Stream(const StreamAddress& stream, const char* interface, unsigned tmo=TIMEOUT);     
public:
  Stream()              = delete;
  Stream(const Stream&) = delete;
public:
 ~Stream();
public: 
  const StreamAddress& stream() const {return _stream;}
public:
  void set(unsigned tmo=0) {_set(tmo);}
private:
  StreamAddress _stream;
  }; 
                 
}

#endif

