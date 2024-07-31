
#ifndef SCS_STATS
#define SCS_STATS

#include <inttypes.h>

namespace SCS {

class __attribute__((__packed__)) Stats {
 public:
  Stats() {clear();};
  Stats(const Stats&);
 public:
  ~Stats() {};
 public:
  Stats& operator=(const Stats&);
 public:
  void clear();
 public:
  void dump(unsigned slot, bool header=true) const;
 private:
  friend class SyncCommand;
 private:
  uint32_t _sent;
  uint32_t _to_bp;
  uint32_t _fr_bp;
  uint32_t _to_dpm;
  uint32_t _last_word;
  uint32_t _pad; // Align to quadword boundary...
};

}

#endif
