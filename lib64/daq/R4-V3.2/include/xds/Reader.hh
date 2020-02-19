
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
 
#ifndef DSS_BUCKET
#define DSS_BUCKET

#include "dcs/Id.hh"
#include "dss/Exception.hh"
#include "dss/Store.hh"
#include "dss/Page.hh"
#include "dss/Pending.hh"

namespace DSS {

class Reader {
public:
  Reader() = delete; 
  Reader(const Bid&, Store&);                                                       
public:
 ~Reader();
public:
  uint32_t size() const;
public:
  int transfer(char* content, uint32_t type, uint32_t length);
private:
  Pending  _pending;
  uint32_t _current;
  uint64_t _bid;
  Store    _store;
  };

}


#endif

