/*
** ++
**  Package: SATA
**	
**
**  Abstract: LSST Storage Sector Interface
**
**  Implementation of LSST Storage Sector Interface
**
**  Author:
**      S. Maldonado, SLAC (smaldona@slac.stanford.edu)
**
**  Creation Date:
**	000 - Sept 15, 2018
**
**  Revision History:
**	None.
**
** --
*/
 
#ifndef SATA_LOG
#define SATA_LOG

#include <stdint.h>

namespace SATA {

class  __attribute__((aligned (32))) __attribute__((packed)) Log  {
public:
  enum {CONDITION_MET=0+3, DSN=1+3, NORMALIZED=2+3, VALID=3+3, SUPPORTED=4+3};  
public:
  static uint8_t  flags(uint64_t statistic) {return statistic >> 56;}
  static uint64_t value(uint64_t statistic) {return statistic & 0x0FFFFFFFFFFFFFFFll;}   
  static void     dump(uint8_t flags, int indent);
public:  
  Log(uint8_t address, uint8_t page) : _address(address), _page(page) {}
public:
  Log()           = delete;
  Log(const Log&) = delete;  
public:
 ~Log() {}
public:
  uint8_t page()    const {return _page;} 
  uint8_t address() const {return _address;} 
protected:  
  uint64_t _statistic(int offset) const {return _buffer[offset];} 
public:
  void* _result() {return _buffer;}  
private:
  uint64_t _buffer[512/sizeof(uint64_t)]; 
  uint8_t  _crc[30];
  uint8_t  _address;
  uint8_t  _page;
  };

static_assert(sizeof(Log) == 512+32, "Class SATA::Log is not equal to one sector (512 bytes)");

}

#endif 
