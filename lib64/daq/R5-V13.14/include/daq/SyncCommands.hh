#ifndef DAQ_SYNC_COMMANDS
#define DAQ_SYNC_COMMANDS

#include <stdint.h>

namespace DAQ {

class SyncCommands
{
public:
  SyncCommands(uint8_t pid, uint32_t link);
public:
  ~SyncCommands() {};
public:
  void enable();
  void disable();
public:
  bool command (uint8_t cmd);
  void enable  (uint8_t cmd);
  void disable (uint8_t cmd);
private:
  volatile uint32_t* _part_reg;
  volatile uint32_t* _cmd_reg;
};
}


#endif
