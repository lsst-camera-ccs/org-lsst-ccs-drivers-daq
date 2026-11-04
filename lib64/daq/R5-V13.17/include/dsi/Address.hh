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
**	000 - January 09, 2007
**
**  Revision History:
**	None.
**
** --
*/
 
#ifndef DSI_ADDRESS
#define DSI_ADDRESS

#include "net/ipv4/Address.hh"

namespace DSI {

class __attribute__((__packed__)) Address : public IPV4::Address {
public:
  enum : uint16_t {PORT    = 29000};      // FIRST allocated port number... 
  enum : uint32_t {ADDRESS = 0xEFFF0000}; // FIRST allocated multicast address(239.255.0.0)...
public:
  enum {BLOCKS2 = 5};              // Number of reserved blocks (expressed as a power of 2)
  enum {BLOCKS  = 1 << BLOCKS2};   // Number of reserved blocks
public:
  enum {OFFSETS2 = 5};             // Number of offsets within a block (expressed as a power of 2)
  enum {OFFSETS  = 1 << OFFSETS2}; // Number of offsets within a block
public:  
  enum {RESERVED = OFFSETS * BLOCKS}; // Total number of multicast addresses and ports reserved...
public:
  Address(uint8_t block, uint8_t offset);
  Address(const Address& clone) : IPV4::Address(clone) {}
public:
  Address() = delete;
public: 
  Address& operator=(const Address& clone) { IPV4::Address::operator=(clone); return *this;}      
public:
 ~Address() {} 
public:
  uint8_t block()  const;
  uint8_t offset() const;
private:  
  uint32_t _group(uint32_t block, uint32_t offset);
  uint16_t _port( uint16_t block, uint16_t offset);
};

}

#endif
