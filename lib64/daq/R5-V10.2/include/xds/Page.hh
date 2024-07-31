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

#include "dvi/Endian.hh"
#include "xds/Id.hh" 

#include "xds/Sector.hh"
#include "xds/Stripe.hh"

namespace XDS {

class __attribute__((__packed__)) Page {
public:
  enum {LENGTH = 3};                     // Page size in units of slices...
  enum {SIZE   = LENGTH * Stripe::SIZE}; // Page Size in units of bytes...
public:
  Page();
  Page(const Id&);
  Page(const Id&, uint32_t number); // As a page number...
  Page(const Id&, uint64_t offset); // As a byte offset...
  Page(const Page&);
public:
 ~Page() {}
public:  
  Page& operator=(const Page& clone);
public:
  Page& operator++()    {uint32_t current = DVI::Endian::local(_number); ++current; _number = DVI::Endian::wire(current); return *this;} // Prefix...
  Page& operator++(int) {uint32_t current = DVI::Endian::local(_number); current++; _number = DVI::Endian::wire(current); return *this;} // Postfix...
public:
  Page& operator+=(unsigned number) {uint32_t current = DVI::Endian::local(_number); current += number; _number = DVI::Endian::wire(current); return *this;}
public:
  uint64_t  id()                        const; // Hash of bucket ID and offset (_number)...
  const Id& bucket()                    const {return _bucket;}  
  uint32_t  number()                    const {return DVI::Endian::local(_number);}
  uint32_t  offset(uint64_t abs_offset) const {return abs_offset - ((uint64_t)number() * SIZE);}
public:
  uint64_t advance();
  uint64_t advance(unsigned number);
private:
  Id       _bucket; // Bucket ID
  uint32_t _number; // Page offset within bucket
};

static_assert(!(sizeof(Page) & 7), "Class XDS::Page is not an even number of quadwords");

}
 
#endif

