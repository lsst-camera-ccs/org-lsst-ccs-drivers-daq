/*
** ++
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
**	000 - , 2007
**
**  Revision History:
**	None.
**
** --
*/
 
#ifndef IPV4_ADDRESS
#define IPV4_ADDRESS

#include <stdint.h>

#include "dvi/Endian.hh"
#include "net/link/Address.hh"
#include "net/ipv4/Exception.hh"

namespace IPV4 {
   
class __attribute__((__packed__)) Address {
public:
  static char* presentation(uint32_t address, char* buffer); 
public:
  Address();
  Address(uint32_t    address, uint16_t port=0);                    // address is in wire endian, port is in local endian
  Address(const char* address, uint16_t port=0);                    // port is in local endian  
  Address(uint32_t    address,                const uint8_t* link); // address is in wire endian
  Address(uint32_t    address, uint16_t port, const uint8_t* link); // address is in wire endian, port is in local endian 
public: 
  Address(const Address&);  
  Address(const Address&, uint16_t port); 
public:  
  const Address& operator=(const Address& clone); 
  const Address& operator=(uint16_t port) {_address.ipv4._port = port; return *this;} // Assumes port is in wire order...
  const Address& operator=(uint32_t addr) {_address.ipv4._addr = addr; return *this;} // Assumes address is in wire order... 
public:  
  bool operator==(const Address& operand) {return operand._address.ipv4._addr == _address.ipv4._addr;} 
public:
 ~Address() {} 
public: 
  operator uint32_t() const {return _address.ipv4._addr;}    
public:
  operator bool() const {return _address.opaque != 0 ;} 
public:
  uint32_t             addr()  const {return _address.ipv4._addr;}
  uint16_t             port()  const {return _address.ipv4._port;} 
  const Link::Address& link()  const {return _link;}  
public:
  uint16_t local()     const {return DVI::Endian::local(_address.ipv4._addr);}
  uint16_t portLocal() const {return DVI::Endian::local(_address.ipv4._port);}     
public:
  char* presentation(char* buffer)     const;
  char* portPresentation(char* buffer) const;
public:
  void print(int indent=0) const;
public:   
  void _recache();
public:
   unsigned family() const {return _address.ipv4._family;} 
private:   
  struct __attribute__((__packed__)) _Address {uint16_t _family; uint16_t _port; uint32_t _addr;};    
private:  
 union {struct _Address ipv4; uint64_t opaque;} _address;
 Link::Address                                  _link; 
};  

}

#endif
