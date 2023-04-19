 
#ifndef RDS_STATS
#define RDS_STATS

#include <stdint.h>

namespace RDS {

class Stats
{
  friend class Plugin;
  friend class RebPlugin;
  friend class DummyPlugin;

public:
  Stats() {clear();};
  Stats(const Stats& clone);
public:  
  ~Stats(){};

public:
  Stats& operator=(const Stats&);

public:
  uint32_t wakes()           const {return _wakes;}
  uint32_t discards()        const {return _discards;}
  uint32_t no_event()        const {return _no_event;}
  uint32_t bdi_waits()       const {return _bdi_waits;}
  uint32_t bdi_nulls()       const {return _bdi_nulls;}
  uint32_t bdi_seq_errors()  const {return _bdi_seq_errors;}
  uint32_t bdi_overflows()   const {return _bdi_overflows;}
  uint32_t bdi_firsts()      const {return _bdi_firsts;}
  uint32_t bdi_lasts()       const {return _bdi_lasts;}
  uint32_t bdi_frees()       const {return _bdi_frees;}

public:
  void dump(const char* source, bool header=false) const;
  void clear();

private:
  uint32_t _wakes;
  uint32_t _discards;
  uint32_t _bdi_waits;
  uint32_t _bdi_nulls;
  uint32_t _bdi_seq_errors;
  uint32_t _bdi_overflows;
  uint32_t _no_event;
  uint32_t _bdi_firsts;
  uint32_t _bdi_lasts;
  uint32_t _bdi_frees;            
  
}; 
  
}

#endif
