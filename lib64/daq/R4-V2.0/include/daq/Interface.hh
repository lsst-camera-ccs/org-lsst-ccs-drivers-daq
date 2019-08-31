
/*
**  Package:
**	
**
**  Abstract:
**      
**
**  Author:
**      Michael Huffer, SLAC (mehsys@slac.stanford.edu)
**
**  Creation Date:
**	    000 - April 06, 2011
**
**  Revision History:
**	    None.
**
** --
*/
 
#ifndef DAQ_INTERFACE
#define DAQ_INTERFACE

#include "daq/Location.hh"
#include "daq/Sensor.hh"
#include "daq/Lane.hh"

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
  Interface(unsigned index, Location& name, Lane::Type lane);
  Interface(const Interface& clone) : _index(clone._index), _lane(clone._lane), _link(clone._link), _name(clone._name), _type(clone._type) {}
public:
 ~Interface() {}
public:
  Interface& operator=(const Interface& clone) {_index = clone._index; _lane = clone._lane; _link = clone._link, _name = clone._name; _type = clone._type; return *this;} 
public:
  unsigned        index()    const {return _index;}
  const Location& name()     const {return _name;} 
  Lane::Type      lane()     const {return _lane;} 
  unsigned        link()     const {return _link;}
  Sensor::Type    type()     const {return _type;}
public:
  void stats(InterfaceFirmwareStats&, bool clear=false) const;
  void stats(InterfaceDriverStats&,   bool clear=false) const;
public:
  bool online() const;
public:
  void dump(int indent=0) const;
private:
  Sensor::Type _lookup(const Location&);
private:
  unsigned     _index;
  Lane::Type   _lane;
  uint32_t     _link;
  Location     _name; 
  Sensor::Type _type; 
 };

}

#endif

