#ifndef DAQ_INTERFACEFIRMWARESTATS
#define DAQ_INTERFACEFIRMWARESTATS

#include <stdint.h>

namespace DAQ {

class InterfaceFirmwareStats
{
  friend class Interface;
public:
  enum {NUMOF_VCS=4};
public:
  InterfaceFirmwareStats() {};
  InterfaceFirmwareStats(const InterfaceFirmwareStats&);
public:
  ~InterfaceFirmwareStats() {};
public:
  InterfaceFirmwareStats& operator=(const InterfaceFirmwareStats&);
public:
  uint32_t link_status()           const {return _link_status;}
  uint32_t rlink_data()            const {return _rlink_data;}
  uint32_t cell_errors()           const {return _cell_errors;}
  uint32_t link_down()             const {return _link_down;}
  uint32_t link_errors()           const {return _link_errors;}
  uint32_t overflow_remote(int vc) const {return _overflow_remote[vc];}
  uint32_t rx_errors()             const {return _rx_errors;}
  uint32_t rx_frames()             const {return _rx_frames;}
  uint32_t overflow_local(int vc)  const {return _overflow_local[vc];}
  uint32_t tx_errors()             const {return _tx_errors;}
  uint32_t tx_frames()             const {return _tx_frames;}
  uint32_t rx_clock()              const {return _rx_clock;}
  uint32_t tx_clock()              const {return _tx_clock;}
  uint32_t last_tx()               const {return _last_tx;}
  uint32_t last_rx()               const {return _last_rx;}
  uint32_t tx_op()                 const {return _tx_op;}
  uint32_t rx_op()                 const {return _rx_op;}
public:
  void dump(const char* source, bool header = false) const;
  void dump_full (const char* source) const;
private:
  // These are the statistics collected by the PGP firmware
  uint32_t _link_status;                // Link Status
  uint32_t _rlink_data;                 // Remote Link Data
  uint32_t _cell_errors;                // Cell Error Count
  uint32_t _link_down;                  // Link Down Count
  uint32_t _link_errors;                // Link Error Count
  uint32_t _overflow_remote[NUMOF_VCS]; // Remote Overflow VC Count
  uint32_t _rx_errors;                  // Receive Frame Error Count
  uint32_t _rx_frames;                  // Receive Frame Count
  uint32_t _overflow_local[NUMOF_VCS];  // Local Overflow VC Count
  uint32_t _tx_errors;                  // Transmit Frame Error Count
  uint32_t _tx_frames;                  // Transmit Frame Count
  uint32_t _rx_clock;                   // Receive Clock Frequency
  uint32_t _tx_clock;                   // Transmit Clock Frequency
  uint32_t _last_tx;                    // Last OpCode Transmitted
  uint32_t _last_rx;                    // Last OpCode Received
  uint32_t _tx_op;                      // OpCode Transmit count
  uint32_t _rx_op;                      // OpCode Received count
};
  
}


#endif
