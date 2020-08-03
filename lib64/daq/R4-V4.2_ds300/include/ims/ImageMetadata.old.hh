
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
 
#ifndef IMS_IMAGEMETADATA
#define IMS_IMAGEMETADATA

#include "daq/LocationSet.hh"
#include "osa/TimeStamp.hh"
#include "dvi/Version.hh"
#include "ims/Id.hh"

namespace IMS {
 
class __attribute__((__packed__)) ImageMetadata {
public:
  enum {MAX=63}; // maximum # of characters for either a name or folder (exclusive of NULL termination)...
public:
  ImageMetadata();
  ImageMetadata(const char* name, const DAQ::LocationSet&, int32_t opcode, const char* annotation = (const char*)0); 
  ImageMetadata(const ImageMetadata&);
public:
 ~ImageMetadata() {}
public: 
  ImageMetadata& operator=(const ImageMetadata&);    
public:
  operator bool() const;
public:
  const char*             name()       const;
  const char*             annotation() const;
  const DAQ::LocationSet& elements()   const; 
  const OSA::TimeStamp&   timestamp()  const; 
  const DVI::Version&     release()    const;
  int32_t                 opcode()     const;
public:
  void erase();  
  void reset(OSA::TimeStamp& timestamp) {_timestamp = timestamp;}
private:
  DAQ::LocationSet _elements;
  OSA::TimeStamp   _timestamp;
  DVI::Version     _release;
  int32_t          _opcode;
  uint32_t         _pad;
  char             _name[MAX+1];
  char             _annotation[MAX+1];
 };

}

#endif

