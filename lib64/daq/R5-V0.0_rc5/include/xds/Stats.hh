
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
#include "dsi/Location.hh"

namespace XDS {

class __attribute__((__packed__)) Stats {
public: 
  Stats(); 
  Stats(uint32_t capacity, unsigned remaining); 
  Stats(const Stats&);
public:
 ~Stats() {} 
public: 
  Stats& operator=(const Stats&);
  Stats& operator+=(const Stats&);
public: 
  Stat capacity;
  Stat remaining;
  Stat wakeups;
  Stat reads;
  Stat writes;
  Stat lookups;
  Stat removes;
public:
  void reset();
public:  
  void dump(const DSI::Location&, int indent=0) const;
  void dump(const char* title,    int indent=0) const;
private:
  void _dump(const char* title, int indent=0) const;
private:
  uint32_t pad;  
  };

}

#endif

