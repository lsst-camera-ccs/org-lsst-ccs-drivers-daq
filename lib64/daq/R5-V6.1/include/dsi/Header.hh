
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
 
#ifndef DSI_HEADER
#define DSI_HEADER

#include <stdio.h>
#include <stdint.h>

#include "dvi/Endian.hh"
#include "dsi/Set.hh"

namespace DSI {

class __attribute__((__packed__)) Header {
public:
  enum Function : uint8_t {F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24, F25, F26, F27, LOOKUP, PROBE, RESTART, SHUTDOWN, UNDEFINED};
public:
  Header()                                                                         : _sequence(0),               _function(Function::UNDEFINED), _length(0),             _user(0),           _accept()                     {}      
  Header(enum Function function, size_t length,                    uint8_t user=0) : _sequence(0),               _function(function),            _length(length),        _user(user),        _accept(Set(Set::State::ANY)) {}   
  Header(enum Function function, size_t length, const Set& accept, uint8_t user=0) : _sequence(0),               _function(function),            _length(length),        _user(user),        _accept(accept)               {}
  Header(const Header& clone)                                                      : _sequence(clone._sequence), _function(clone._function),     _length(clone._length), _user(clone._user), _accept(clone._accept)        {}
  Header(const Header& clone, size_t length, uint8_t user=0)                       : _sequence(clone._sequence), _function(clone._function),     _length(length),        _user(user),        _accept(clone._accept)        {}
public:
 ~Header() {} 
public:  
  Header& operator=(const Header& clone) {_sequence = clone._sequence; _function = clone._function; _length = clone._length, _accept = clone._accept; return *this;}
  Header& operator=(uint32_t sequence)   {_sequence = DVI::Endian::wire(sequence); return *this;} 
public:  
  bool operator==(const Header& request) const {return request._sequence == _sequence;} 
  bool operator!=(const Header& request) const {return request._sequence != _sequence;}  
public:
  void filter(const Set& accept) {_accept &= accept;}
public: 
  uint32_t sequence() const {return DVI::Endian::local(_sequence);}
  uint8_t  function() const {return _function;}
  uint8_t  length()   const {return _length;}
public:  
  const Set& accepted() const {return _accept;}
  Set&       servers()        {return _accept;}
protected:
  uint8_t user() const {return _user;}
private:
  uint32_t _sequence;
  uint8_t  _function; 
  uint8_t  _length;
  uint8_t  _user;
  uint8_t  _pad; 
  Set      _accept;
  };

static_assert(!(sizeof(Header) & 7), "Class DSI::Header is not an even number of quadwords");

}

#endif

