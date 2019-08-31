
#ifndef SCS_STATS
#define SCS_STATS

#include <inttypes.h>
#include <string.h>

namespace SCS {

class __attribute__((__packed__)) Stats
{
  
friend class SyncCommand;
 
public:
  Stats() { clear(); };
  Stats(const Stats&);
public:
  ~Stats() {};
public:
  Stats& operator=(const Stats&);
public:
  void dump(unsigned indent=0);
public:
  void clear() { memset(this, sizeof(Stats), 0); }
private:
  uint32_t _sent;
  uint32_t _to_bp;
  uint32_t _fr_bp;
  uint32_t _to_dpm;
  uint32_t _last_word;
};

}

#endif
