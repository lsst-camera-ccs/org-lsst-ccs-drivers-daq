
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
 
#ifndef XDS_WRITER
#define XDS_WRITER

#include "dcs/Id.hh"
#include "dss/Exception.hh"
#include "dss/Store.hh"
#include "dss/Page.hh"
#include "dss/Pending.hh"

namespace DSS {

class Writer {
public:
  Writer() = delete; 
  Writer(const Bid&, Store&);                                                       
public:
 ~Writer();
public:
  int transfer(const char* content, uint32_t type, uint32_t length);
private:
  Pending  _pending;
  uint32_t     _current;
  uint64_t _bid;
  Store&   _store;
  };

}


#endif

