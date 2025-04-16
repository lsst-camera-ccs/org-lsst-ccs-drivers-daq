
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

#include "xds/Stream.hh"
#include "xds/Id.hh"
#include "ims/Store.hh"
 
namespace IMS {

class Stream : public XDS::Stream {
public: 
 Stream(Store& store, unsigned timout=0) : XDS::Stream(XDS::Id::Space::IMS, store, timout) {} 
public:
  Stream()              = delete;
  Stream(const Stream&) = delete;
public:  
 ~Stream() {}
  }; 

}

#endif


