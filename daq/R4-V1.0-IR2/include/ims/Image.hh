
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
  Image() = delete; 
  Image(const Id&, Store&);
  Image(Store&, Stream&, int timout=0);
  Image(const char* folder, const ImageMetadata&, Store&);
  Image(const Id&, const ImageMetadata&, const char* folder, Store&);
  Image(const Image&);  
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
  Stream* _wait() {Stream* stream = _stream; _stream = (Stream*)0; return stream;}
private:
  Store&        _store;
  Stream*       _stream;
  ImageMetadata _metadata;
  Id            _id;
  int32_t       _error;
};

}

#endif

