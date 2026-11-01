
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
** 
*/
 
#ifndef DSM_STREAM
#define DSM_STREAM

#include "dsi/Stream.hh"
#include "dsm/Streams.hh"

namespace DSM {

class Stream : public DSI::Stream {
public:
  enum {TIMEOUT = 4}; // Default wait timeout in 10 ms tics (40 milliseconds)...   
public:
  Stream(Streams::Id, const char* partition, unsigned tmo=TIMEOUT); 
  Stream(Streams::Id, uint8_t     partition, unsigned tmo=TIMEOUT); 
public:
  Stream(Streams::Id, const char* partition, const char* interface, unsigned tmo=TIMEOUT); 
  Stream(Streams::Id, uint8_t     partition, const char* interface, unsigned tmo=TIMEOUT);   
public: 
  Stream()        = delete;
  Stream(Stream&) = delete;
public:
 ~Stream() {}
private: 
  uint8_t _lookup(const char* partition); 
};
                 
}

#endif

