/*
** ++
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
**	000 - January 09, 2007
**
**  Revision History:
**	None.
**
** --
*/
 
#ifndef XDS_PAGE
#define XDS_PAGE

#include "net/Endian.hh"
#include "xds/Id.hh" 

namespace XDS {

class __attribute__((__packed__)) Page {
public:
 enum {SIZE2=20};
 enum {SIZE=1<<SIZE2};
 enum {REMAINDER=SIZE-1};
public:
  Page();
  Page(const Id&);
  Page(const Id&,   uint64_t offset, uint64_t length, uint32_t publish=0);
  Page(uint64_t id, uint64_t offset, uint64_t length, uint32_t publish=0);
  Page(const Id&, uint64_t length); // set EOB...
  Page(const Page&);
public:
 ~Page() {}
public:  
  Page& operator=(const Page& clone);
public:
  uint64_t  id()      const {return Net::Endian::local(_id);} 
  uint64_t  offset()  const {return Net::Endian::local(_offset);}    
  uint64_t  length()  const {return Net::Endian::local(_length);}    
  const Id& bucket()  const {return _bucket;}  
public:
  char* content() const {return (char*)&this[1];}
public:
  operator bool() const {return _length;}
public:
  bool publish() const {return _publish;}
public:
  void dump(int indent=0) const;   
public:
  void posion() {_length = uint64_t(0);}
private:  
  uint64_t _hash() const; 
private:
  Id       _bucket;
  uint32_t _publish;
  uint64_t _offset;
  uint64_t _length;
  uint64_t _id;
};

}
 
#endif

