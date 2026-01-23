
#ifndef DAQ_INTERFACE
#define DAQ_INTERFACE

#include "daq/Location.hh"
#include "daq/Sensor.hh"
#include "daq/Path.hh"

namespace DAQ {

class InterfaceDriverStats;
class InterfaceFirmwareStats;

class  Interface {
public:
  enum Index {ZERO=0, ONE=1, TWO=2};
public:
  static const char* header();
public:
  Interface();
  Interface(uint8_t index, Location& name, Path::LANE lane);
  Interface(const Interface& clone) : _index(clone._index), _name(clone._name), _lane(clone._lane),  _type(clone._type), _link(clone._link) {}
public:
 ~Interface() {}
public:
  Interface& operator=(const Interface& clone) {_index = clone._index; _name = clone._name; _lane = clone._lane; _type = clone._type; _link = clone._link; return *this;}
public:
  uint8_t         index() const {return _index;}
  const Location& name()  const {return _name;}
  Path::LANE      lane()  const {return _lane;}
  Sensor::Type    type()  const {return _type;}
  uint32_t        link()  const {return _link;}
public:
  void stats(InterfaceFirmwareStats&, bool clear=false) const;
  void stats(InterfaceDriverStats&,   bool clear=false) const;
public:
  bool online() const;
public:
  void reset() const;
public:
  void dump(int indent=0) const;
private:
  Sensor::Type _lookup(const Location&);
private:
  uint8_t      _index;
  Location     _name;
  Path::LANE   _lane;
  Sensor::Type _type;
  uint32_t     _link;
 };

}

#endif

