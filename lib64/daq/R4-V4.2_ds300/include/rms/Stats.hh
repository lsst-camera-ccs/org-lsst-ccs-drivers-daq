 
#ifndef RMS_STATS
#define RMS_STATS

#include <stdint.h>

namespace RMS {
  
class Plugin;

class Stats
{
  friend class Plugin;

public:
  Stats(){ clear();};
  Stats(const Stats& clone);
  
public:  
  ~Stats(){};

public:
  Stats& operator=(const Stats&);

public:
  uint32_t probes()       const { return _probes; }	   
  uint32_t unopened()     const { return _unopened; }	   
  uint32_t in_reset()     const { return _in_reset; }	   
  uint32_t link_down()    const { return _link_down; }
  uint32_t lists_posted() const { return _lists_posted; }
  uint32_t ops_posted()   const { return _ops_posted; }
  uint32_t waits()        const { return _waits; }	   
  uint32_t responses()    const { return _responses; }
  uint32_t inv_tids()     const { return _inv_tids; }
  uint32_t orphans()      const { return _orphans; }
  uint32_t rundowns()     const { return _rundowns; }     
  uint32_t tid_hwm()      const { return _tid_hwm; }     

public:
  void dump(const char* source, bool header = false) const;

private:
  void clear()
  {
    _probes       = 0;
    _unopened     = 0;
    _in_reset     = 0;
    _link_down    = 0;
    _lists_posted = 0;
    _ops_posted   = 0;
    _waits        = 0;
    _responses    = 0;
    _inv_tids     = 0;
    _orphans      = 0;
    _rundowns     = 0;
    _tid_hwm      = 0;
  }

private:
  uint32_t _probes;
  uint32_t _unopened;
  uint32_t _resets;
  uint32_t _reset_tmo;
  uint32_t _in_reset;
  uint32_t _link_down;

  uint32_t _lists_posted;
  uint32_t _ops_posted;

  uint32_t _waits;
  uint32_t _responses;
  uint32_t _inv_tids;
  uint32_t _orphans;
  uint32_t _rundowns;      

  uint32_t _tid_hwm;
}; 
  
}

#endif
