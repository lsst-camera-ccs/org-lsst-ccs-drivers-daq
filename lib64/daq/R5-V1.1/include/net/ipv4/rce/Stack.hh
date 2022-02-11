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
 
#ifndef IPV4_STACK
#define IPV4_STACK

#include <stdint.h>    // Appears to be necessary for SAS include to compile under all cases...
#include "sas/Sas.h"

#include "net/ipv4/Address.hh"
#include "net/ipv4/Counters.hh"

namespace IPV4 {

class Accept; // Forward reference...

class Stack {
public:
  Stack()             = delete;
  Stack(const Stack&) = delete;
public:
 ~Stack() {}
public: 
 static void startup();
public: 
 static const Counters& counters();
public:  
  static bool insert(const Accept&);
  static bool remove(const Address&);
  static bool remove(uint16_t port);
public:
  static SAS_MbxId mid();
};

}

#endif