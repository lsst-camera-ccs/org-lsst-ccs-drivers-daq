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

#ifndef IPV4_DATAGRAM
#define IPV4_DATAGRAM

#include "net/ipv4/Address.hh"
#include "net/ipv4/Header.hh"
#include "net/ipv4/Payload.hh"

namespace IPV4 {

class Datagram {
public:
  enum {MAX_HEADER   = 10};      // In units of quadwords (8 bytes)...
  enum {MAX_DATAGRAM = 1024*64}; // In units of bytes...
public:
  static void* operator new(size_t size, char* buffer) {return (void*)buffer;}
public:  
  Datagram(const Address&);
  Datagram(const Address&, const Header&);
  Datagram(const Address&, const Payload&);
  Datagram(const Address&, const Header&, const Payload&);
public:  
  Datagram(const Address&, void* header, size_t length);
  Datagram(const Address&, void* header, size_t length, const Payload&);
public:
  Datagram(const Header&);
  Datagram(const Header&, const Payload&); 
  Datagram(const Payload&);
public:
  Datagram(void* header, size_t length);
  Datagram(void* header, size_t length, const Payload&); 
public:
  Datagram(const Datagram&);
public:
  Datagram() = delete;
public:
 ~Datagram() {}
public:  
  const Datagram& operator=(const Datagram&);
public:
  size_t length() const {return header.length() + payload.length();}  
public:
  Address& source()      {return _address;}
  Address& destination() {return _address;}
public:
  void dump() const;
public:
  Payload payload;  
  Header  header;
private:
  Address _address;  
};

}

#endif