
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
 
#ifndef XDS_STATS
#define XDS_STATS

#include "xds/Stat.hh"

namespace XDS {

class Stats {
public: 
  Stats(); 
  Stats(uint64_t capacity); 
  Stats(const Stats&);
public:
 ~Stats() {} 
public: 
  Stats& operator=(const Stats&);
  Stats& operator+=(const Stats&);
public: 
  Stat capacity;
  Stat remaining;
  Stat lookups;
  Stat gets;
  Stat inserts;
  Stat terminates;
  Stat removes;
  Stat resets;
public:
  void reset();
  };

}

#endif

