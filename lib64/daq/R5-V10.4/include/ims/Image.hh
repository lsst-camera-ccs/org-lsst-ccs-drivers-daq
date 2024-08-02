
/*
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
**	    000 - April 06, 2011
**
**  Revision History:
**	    None.
**
** --
*/
 
#ifndef IMS_IMAGE
#define IMS_IMAGE

#include "ims/Id.hh"
#include "ims/Store.hh"
#include "ims/Stream.hh"
#include "ims/ImageMetadata.hh"
 
namespace IMS {
  
class Image {
public:  
  static void title(int indent=0);
public:
  Image(const Id&, Store&);
  Image(Store&, Stream&, int timout=0);
  Image(const ImageMetadata&, Store&);
  Image(const Id&, const ImageMetadata&, Store&);
  Image(const Image&);  
public:
  Image() = delete; 
public:
 ~Image() {}
public:  
  Image& operator=(const Image&);
public:
  operator bool() const;
public:
  Store& store();
public:
  const Id&            id()       const;
  const ImageMetadata& metadata() const;
public:   
  void synopsis(int indent=0) const;
public:
  int32_t remove();
  int32_t moveTo(const char* folder);
public:  
  int32_t error() const;
private:
  friend class Remover; 
  friend class Decoder; 
private:
  Id            _id;
  int32_t       _error;
  Store&        _store;
  ImageMetadata _metadata;
};

}

#endif

