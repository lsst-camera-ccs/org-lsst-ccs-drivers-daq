
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
 
#ifndef XDS_STREAM
#define XDS_STREAM

#include "dsm/Stream.hh"
#include "xds/Event.hh"
#include "xds/Id.hh"
#include "xds/Store.hh"

namespace XDS {

class Stream : public DSM::Stream {
public:                         
  Stream(Id::Space, const char* partition, unsigned timout=0);  
  Stream(Id::Space, uint8_t     partition, unsigned timout=0); 
public:                         
  Stream(Id::Space, const char* partition, const char* interface, unsigned timout=0);  
  Stream(Id::Space, uint8_t     partition, const char* interface, unsigned timout=0); 
public:
  Stream(Id::Space, Store&, unsigned timout=0);  
public:  
  Stream()              = delete;
  Stream(const Stream&) = delete;
public:  
 ~Stream() {}
public:
  bool wait(Event&); 
  bool wait(uint8_t types, Event&); 
  bool wait(uint8_t types, uint64_t id, Event&); 
private:
  size_t _header(const uint64_t opaque_header[]) const {return sizeof(Event);}
private:
   Id::Space _space;  
  }; 

}

#endif

