#ifndef GDS_STATEMETADATA
#define GDS_STATEMETADATA

#include <stdint.h>

#include "osa/TimeStamp.hh"
#include "gds/Location.hh"

namespace GDS {
  
class __attribute__((__packed__)) StateMetadata
{
public:
  enum Type   : uint8_t {STAMP=0, START=1, STOP=2, PAUSE=3, RESUME=4, STATUS=5};
  enum Status : uint8_t {SUCCESS=0, BDI_ERROR=1, MAX_SUBSCRIBERS=2};
public:
  static const char* type(Type);
  static const char* status(Status);
public:
  StateMetadata(const Type, uint16_t sequence, const OSA::TimeStamp&, const Location&, const char* comment, const Status status=SUCCESS);
  StateMetadata(const StateMetadata&);
  StateMetadata() {};//= delete;
public:
  ~StateMetadata() {};
public:
  StateMetadata& operator=(const StateMetadata&);
public:
  void setStatus(const Status& status) {_status = status;}
public:
  const Type&           type()      const {return _type;}
  const Status&         status()    const {return _status;}
  uint16_t              sequence()  const {return _sequence;}
  const OSA::TimeStamp& timestamp() const {return _time;}
  const Location&       sensor()    const {return _sensor;}
  const char*           comment()   const {return _comment;}
public:
  void dump(int indent=0) const;
  const char* encode() const;
private:
  Type           _type;
  Status         _status;
  uint16_t       _sequence;
  OSA::TimeStamp _time;
  Location       _sensor;
  uint8_t        _pad;
  char           _comment[32];
};

}

#endif
