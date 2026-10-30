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
 
#ifndef DSI_STREAMADDRESS
#define DSI_STREAMADDRESS

#include "net/ipv4/Address.hh"
#include "dsi/Address.hh"

namespace DSI {

class __attribute__((__packed__)) StreamAddress : public IPV4::Address {
public:
  enum : uint16_t {PORT    = DSI::Address::PORT    + DSI::Address::RESERVED}; // FIRST allocated port number... 
  enum : uint32_t {ADDRESS = DSI::Address::ADDRESS + DSI::Address::RESERVED}; // FIRST allocated multicast address(239.255.0.0)...
public:
  enum {OFFSETS2 = 2};             // Number of offsets within a block (expressed as a power of 2)
  enum {OFFSETS  = 1 << OFFSETS2}; // Number of offsets within a block
public:  
  enum {RESERVED = OFFSETS * DSI::Address::BLOCKS}; // Total number of multicast addresses and ports reserved...
public:
  StreamAddress(uint8_t block, uint8_t offset);
  StreamAddress(const StreamAddress& clone) : IPV4::Address(clone) {}
public:
  StreamAddress() = delete;
public: 
  StreamAddress& operator=(const StreamAddress& clone) { IPV4::Address::operator=(clone); return *this;}      
public:
 ~StreamAddress() {} 
public:
  uint8_t block()  const;
  uint8_t offset() const;
private:  
  uint32_t _group(uint32_t block, uint32_t offset);
  uint16_t _port( uint16_t block, uint16_t offset);
};

}

#endif
