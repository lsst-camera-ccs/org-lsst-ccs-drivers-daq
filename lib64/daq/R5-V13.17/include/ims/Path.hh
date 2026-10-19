
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

#ifndef IMS_PATH
#define IMS_PATH

#include "ims/ImageMetadata.hh"

namespace IMS {

class Path {
public: 
  enum {MAX=63+1}; // Inclusive of NULL termination... 
public: 
  Path() = delete;
public:  
  Path(const char* path, const char* default_folder);
  Path(const Path&);                
public:
 ~Path() {} 
public:  
  Path& operator=(const Path&);
  Path& operator=(const char*);
public:  
  operator bool() const {return _valid;}
public:
  const char* image()  const {return _one_token ? _first :  _second;}
  const char* folder() const {return _one_token ? _second : _first;} 
private:
//  void _parse(const char* path, const char* default_folder);    
private:
  int         _valid;
  bool        _one_token;
  char        _first[ ImageMetadata::MAX+1];
  char        _second[ImageMetadata::MAX+1];  
 };

}

#endif

