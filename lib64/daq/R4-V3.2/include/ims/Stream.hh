
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
 
#ifndef IMS_STREAM
#define IMS_STREAM

#include "dsi/Set.hh"
#include "xds/EventList.hh"
#include "xds/Id.hh"
#include "xds/Stream.hh"
#include "ims/Id.hh"
#include "ims/ImageMetadata.hh"
#include "ims/Store.hh"
 
namespace IMS {

class Stream {
public:
  Stream()              = delete;
  Stream(const Stream&) = delete;
public: 
  Stream(Store&, int timout=0); 
public:  
 ~Stream() {}
private:
  friend class Image;
  friend class Decoder; 
private:
  uint64_t    image(ImageMetadata&, int timout=0);   
  XDS::Event* source(uint64_t image, const DSI::Set&); 
  void        timout() {_stream.enable(_timout);}
private:
  uint64_t          _probe(ImageMetadata&);
  XDS::LinkedEvent* _lookup(uint64_t image);
  XDS::LinkedEvent* _pend(uint64_t image, const DSI::Set&);
  bool              _filter(XDS::LinkedEvent*, uint64_t image, const DSI::Set&);
private:
  Store&         _store;
  XDS::EventList _image;
  XDS::EventList _source;
  OSA::TimeStamp _current;
  XDS::Stream    _stream;
  int            _timout;
  }; 

}

#endif


