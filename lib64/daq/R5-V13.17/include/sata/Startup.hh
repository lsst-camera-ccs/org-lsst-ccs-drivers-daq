/*
** ++
**  Package: SMI
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

#ifndef SATA_STARTUP
#define SATA_STARTUP

#define __need_getopt_newlib

#include <getopt.h>

namespace SATA {

class Startup {
public:
  enum {ARGC = 1+1}; 
  enum {RESERVED = 0, PLUGIN = 1};          //  Offsets to argument list for starting up SATA task(s)...
public:
  static void run(int unit, int loops, int max_sectors, uint32_t base);  
public:  
  Startup();
public:
  Startup(const Startup&) = delete;
public:  
  ~Startup() {}
  };
}

#endif 
