
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
 
#ifndef XDS_SUMSTATS
#define XDS_SUMSTATS

#include "dsi/Location.hh"
#include "dsi/LocationSet.hh"
#include "xds/Store.hh"
#include "xds/Stats.hh"

namespace XDS {

class SumStats {
public:  
  SumStats() = delete; 
public:                       
  SumStats(Store& store)          : _store(store)        {}      
  SumStats(const SumStats& clone) : _store(clone._store) {}                         
public:
  virtual ~SumStats() {}
public:
  SumStats& operator=(const SumStats& clone) {_store = clone._store; return *this;};
public:
  void sample(DSI::LocationSet&);
  void reset( DSI::LocationSet&);
public:  
  virtual void process(const DSI::Location&, const Stats&) = 0;
private: 
  Store& _store;
  };

}

#endif

