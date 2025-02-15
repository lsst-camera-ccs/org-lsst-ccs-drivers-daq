#ifndef GDS_CLEARPARAMETERS
#define GDS_CLEARPARAMETERS

#include <stdint.h>

namespace GDS {

class __attribute__((__packed__)) ClearParameters
{
public:
  ClearParameters(uint32_t delay, uint32_t preRows, uint32_t flushCount, uint32_t readRows);
  ClearParameters();
public:
  ClearParameters(const ClearParameters&);
  const ClearParameters& operator=(const ClearParameters&);
public:
  ~ClearParameters() {};
public:
  operator bool() const {return _delay || _preRows || _flushCount || _readRows;}
public:
  void dump(unsigned indent=0) const;
public:
  uint32_t delay()      const {return _delay;}
  uint32_t preRows()    const {return _preRows;}
  uint32_t flushCount() const {return _flushCount;}
  uint32_t readRows()   const {return _readRows;}
private:
  uint32_t _delay;
  uint32_t _preRows;
  uint32_t _flushCount;
  uint32_t _readRows;
};

}

#endif
