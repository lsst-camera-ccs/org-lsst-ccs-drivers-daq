#ifndef REB_EEPROM
#define REB_EEPROM

#include "daq/LocationSet.hh"
#include "reb/Bitfile.hh"
#include "rms/Client.hh"

namespace REB {

class EEprom 
{
public:
  enum Status : uint32_t {OK_PROG=1<<0, OK_ERASE=1<<1, OK_ID=1<<2, OK_INIT=1<<3,
      ERR_SIZE=1<<4, ERR_ADD=1<<5, ERR_TMO=1<<6, ERR_PROG=1<<7,
      ERR_ERASE=1<<8, ERR_ID=1<<9, DONE=1<<10};
  enum : uint32_t {STAT_FAIL = ERR_SIZE | ERR_ADD | ERR_TMO | ERR_PROG | ERR_ERASE | ERR_ID};
  enum : uint32_t {DONE_WORD = 1};
public:
  EEprom(const char* partition, const char* interface) : _rms(partition, interface) {};
  EEprom(const char* partition)                        : _rms(partition) {};
  EEprom(uint8_t     partition, const char* interface) : _rms(partition, interface) {};
  EEprom(uint8_t     partition)                        : _rms(partition) {};
public:
  DAQ::LocationSet boot (const DAQ::LocationSet&, unsigned slot);
public:
  DAQ::LocationSet erase(const DAQ::LocationSet&, unsigned slot, bool final=false);
public:
  DAQ::LocationSet load (const DAQ::LocationSet&, unsigned slot, const char* filename);
  DAQ::LocationSet load (const DAQ::LocationSet&, unsigned slot, const Bitfile&);
private:
  DAQ::LocationSet _init(DAQ::LocationSet&);
private:
  DAQ::LocationSet _version_filter(DAQ::LocationSet&);
  DAQ::LocationSet _type_filter(DAQ::LocationSet&, const Bitfile&);
  DAQ::LocationSet _boot(DAQ::LocationSet&, unsigned slot);
  DAQ::LocationSet _wait(DAQ::LocationSet&);
  DAQ::LocationSet _boot_check(DAQ::LocationSet&);
private:
  DAQ::LocationSet _load(DAQ::LocationSet&, unsigned slot, const Bitfile&);
private:
  DAQ::LocationSet _erase(DAQ::LocationSet&, unsigned slot);
  DAQ::LocationSet _erase_wait(DAQ::LocationSet&);
private:
  DAQ::LocationSet _complete(DAQ::LocationSet&);
  DAQ::LocationSet _status_wait(DAQ::LocationSet&, const Status, unsigned tmo_usec, unsigned exp_sec=0);
private:
  RMS::Client _rms;
};
}
#endif
