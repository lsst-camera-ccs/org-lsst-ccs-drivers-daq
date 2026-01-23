
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
 
#ifndef DSID_STREAM
#define DSID_STREAM

#include "dsi/Set.hh"
#include "dsi/Stream.hh"

namespace DSID {

class Stream : public DSI::Stream {
public: 
  static const char* interface();
public:
  Stream(uint16_t port, const char* partition, const DSI::Set& accept);     
  Stream(uint16_t port, uint8_t     partition, const DSI::Set& accept);     
public:
  Stream()              = delete;
  Stream(const Stream&) = delete;     
public:
  virtual ~Stream() {}
public:
  uint8_t pid() const {return _pid;}
private:
  uint8_t _pid;
  uint8_t _pad[3];
};
                 
}

#endif

