/*
** ++
**  Package: SATA
**	
**
**  Abstract: Storage Manager Plugin Driver
**
**  Implementation of LSST Storage Manager Drive
**
**  Author:
**      M. Huffer (mehsys@slac.stanford.edu) derived from original from 
**      S. Maldonado, SLAC (smaldona@slac.stanford.edu)
**
**  Creation Date:
**	000 - July 20, 2018
**  001 - June 1, 2020
**
**  Revision History:
**	None.
**
** --
*/

#ifndef SATA_DRIVE
#define SATA_DRIVE
 
#define __need_getopt_newlib

#include <getopt.h>
#include <stdint.h>    // Appears to be necessary for SAS include to compile...

#include "sas/Sas.h"
#include "sata/Heap.hh"
#include "sata/Features.hh"
#include "sata/General.hh"
#include "sata/Temperatures.hh"
#include "sata/Idle.hh"
#include "sata/Standby.hh"
#include "sata/Power.hh"
#include "sata/Rundown.hh"
 
namespace SATA {

class Plugin; // Forward reference...
 
class Drive {
public:
  Drive(int unit);  // from 0-2  
public:
  Drive()             = delete;
  Drive(const Drive&) = delete;
public:  
 ~Drive();
public:
  int unit() const {return _unit;} 
public:
  bool identify(Features&); 
  bool read(General&);
  bool read(Temperatures&); 
public:  
  bool set(const Standby&);
  bool set(const Idle&);
public:
  bool probe(Power&);
public:
  bool read( uint32_t offset, uint32_t sectors, void* content);  
  bool write(uint32_t offset, uint32_t sectors, void* content);
  void wait();
public:  
  bool readPost( uint32_t offset, uint32_t sectors, void* content, Rundown* rundown=(Rundown*)0);
  bool writePost(uint32_t offset, uint32_t sectors, void* content, Rundown* rundown=(Rundown*)0);
public:
  void reset_plugin(); // RCE SERDES...
  void reset_serdes(); // RCE firmware...
public:
  int remaining() const {return _heap.remaining();}   
public:
  void dump_pending();
  void nudge();
public:                               // in spirit should be (but cannot be) private...
  void _push(SAS_Message message) {_message = message;}
private:
  Plugin*      _plugin;
  int          _unit;
  SAS_Session  _session;
  SAS_ObMbx    _mbx;
  SAS_Message  _message;
  char*        _region;
  Heap         _heap;
public:
  uint32_t     DEBUG[8];
  };
}

#endif 
