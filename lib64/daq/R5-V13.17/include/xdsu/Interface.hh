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

#ifndef XDSU_INTERFACE
#define XDSU_INTERFACE

#include "xds/Store.hh"
#include "xds/Counters.hh"
#include "xds/TestPattern.hh"

namespace XDSU {

class Interface {
public:  
  static void startup(const char* partition); 
public:
  Interface(const char* partition);
public: 
  Interface()                 = delete; 
  Interface(const Interface&) = delete;
public:  
 ~Interface();
public:
  void probe( int argc, char* argv[]);
  void remove(int argc, char* argv[]);
  void lookup(int argc, char* argv[]);
  void list(  int argc, char* argv[]);
  void read(  int argc, char* argv[]);
  void erase( int argc, char* argv[]);
  void write( int argc, char* argv[]);
  void dump(  int argc, char* argv[]);
private:  
  void _xdump(uint32_t length, uint32_t offset);
private:
  XDS::Store       _store; 
  XDS::Counters    _counters;  
  XDS::TestPattern _pattern;
  XDS::TestPattern _content;
  char*            _zero;
  };
  
}

#endif
