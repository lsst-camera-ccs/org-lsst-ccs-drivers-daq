
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

#include "net/ipv4/Socket.hh"
#include "xds/Id.hh"
#include "xds/StreamAddress.hh"

namespace XDS {

class LinkedEvent; // forward link...

class Stream  {
public:
  Stream(Id::Space, const char* partition, const char* interface=(const char*)0); 
  Stream(Id::Space, unsigned    partition, const char* interface=(const char*)0); 
public:  
  Stream()              = delete;
  Stream(const Stream&) = delete;
public:  
 ~Stream() {delete[] _buffer;}
public:
  LinkedEvent* pend(); 
public:
  void enable(int tmo);        // in 10us tics (0 disables timeout)...  
  void disable() {enable(0);} 
private:
  LinkedEvent* _allocate(); 
private:
  StreamAddress _service;
  IPV4::Socket  _socket;
  char*         _buffer;
  unsigned      _next;
  Id::Space     _space;
  }; 

}

#endif

