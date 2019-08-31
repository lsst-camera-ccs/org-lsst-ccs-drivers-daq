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
 
#ifndef XRCE_NEWPLUGIN
#define XRCE_NEWPLUGIN

#include <stdint.h>    // appears to be necessary for SAS include to compile...
#include "sas/Sas.h"

namespace Net {

class NewPlugin {
public:
  enum {TYPE=0};
public: 
  NewPlugin(const NewPlugin&) = delete;
public:  
  NewPlugin();
public:
 ~NewPlugin();
public: 
  SAS_Frame allocate() {return SAS_ObAlloc(_mbx);}
public:
 SAS_Fd*   fd(SAS_Frame frame) {return SAS_ObFd(frame, _mbx);}
 SAS_MbxId mid()               {return  _mid;}
public: 
  void post(SAS_ObOpcode opcode, SAS_Frame frame, uint32_t length) {SAS_ObPost(opcode, SAS_ObSet(frame, TYPE, length), _mbx);} 
public:
  void wait() {return SAS_Wait(_session);}
private:
  SAS_Session _session;
  SAS_ObMbx   _mbx; 
  SAS_MbxId   _mid;
};

}

#endif
